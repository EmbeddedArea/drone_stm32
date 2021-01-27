using GroundStation.ErrorControls;
using GroundStation.Model;
using SharpDX.DirectInput;
using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Timer = System.Windows.Forms.Timer;

namespace GroundStation
{
    public delegate void UpdateForm(Byte[] buffer);
    public partial class Form1 : Form
    {
        public bool isPortOpen = false;
        bool isJoystickConnected = false;
        bool isDroneConnected = false;
        bool sendingFlag = false;
        public void InitTimer()
        {
            timer1 = new Timer();
            timer1.Tick += new EventHandler(timer1_Tick);
            timer1.Interval = 2000; // in miliseconds
            timer1.Start();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            controller.Setup();
        }

        InitilizeSerialComm com1 = null;//a SerialComm class object.

        InitilizeDevice controller = new InitilizeDevice();
        Kontrol Kontrol = new Kontrol();

        private System.Windows.Forms.Timer timer1;


        //************        // Initializes a new instance of the <see cref="Form1"/> class.
        public Form1()
        {
            InitializeComponent();
            com1 = new InitilizeSerialComm(this);//create a new SerialComm, and pass a reference(Form1)  

        }

        //TODO :SERİAL COMMUNİCATİON : 

        // Updates the text. It will update the form textbox. 
        // If it is neccesary, this method will update txt file at as well.
        public void UpdateText(Byte[] buffer)
        {
            String sentence = String.Format("{0}", Encoding.ASCII.GetString(buffer));
            listView1.Items.Add(sentence);
        }


        // Handles the Click event of the openPortButton control.
        private void openPortButton_Click(object sender, EventArgs e)
        {

            if (isPortOpen == false)
            {
                com1.SetPort(portNumberComboBox.Text, Convert.ToInt32(baudRateComboBox.Text));
                com1.Open();
                isPortOpen = true;
                openPortButton.Text = "Close";
            }
            else
            {
                com1.Close();
                isPortOpen = false;
                openPortButton.Text = "Open";
            }

        }


        // Handles the FormClosed event of the Form1 control.
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(Environment.ExitCode);
        }


        // Handles the TextChanged event of the msgDisplayTextBox control.
        private void msgDisplayTextBox_TextChanged(object sender, EventArgs e)
        {

            if (listView1.Text.Length >= 100000)
                listView1.Text = "";
        }


        // Handles the Click event of the clearButton control.
        private void clearButton_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            string[] SerialPorts = System.IO.Ports.SerialPort.GetPortNames();
            if (SerialPorts.Length > 0)
            {
                portNumberComboBox.Text = SerialPorts[SerialPorts.Length - 1];
            }
            else
            {
                portNumberComboBox.Text = "No ports available!";
            }


            listView1.View = View.Details;
            listView1.GridLines = true;

            listView1.Columns.Add("", 300);

            joystickNameLabel.Visible = false;
            uartConnection();
        }





        private void button1_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are You Sure To Exit Program ?", "Exit", MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                Application.Exit();
                Process.GetCurrentProcess().Kill();
            }
        }

        private void portNumberComboBox_DropDown_1(object sender, EventArgs e)
        {
            string[] SerialPorts = System.IO.Ports.SerialPort.GetPortNames();
            portNumberComboBox.Items.Clear();
            //show all available ports
            foreach (string com in SerialPorts)
            {
                portNumberComboBox.Items.Add(com);
            }
        }


        private void msayarla_Click(object sender, EventArgs e)
        {
            Kontrol.ms = Convert.ToInt32(msNumber.Value);
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Environment.Exit(Environment.ExitCode);
        }


        private void uartConnection()
        {

            UInt16 motorPower = 127, rightAndLeft = 127, upAndDown = 127;
            int dataLengthToSend = 0;
            byte[] data_to_send = new byte[60];
            data_to_send[0] = 0x49;      //Drone ID 
            data_to_send[1] = 0x0A;      //Periodic Data     
            byte ctrl_motor_pwm_values = 0;
            byte ctrl_special_motor_movement = 1;

            byte solaDonsunMu = 0, sagaDonsunMu = 0, sabitKalsinMi = 0;
            bool specialMotorMovement = false;
            float range;
            while (!Kontrol.stop_bool)//forever
            {
                controller.joystick.Poll();  //retrives data from poll object. //anket objesinden datayı geri çeker. //retrive: geri çekmek
                JoystickUpdate[] datas = controller.joystick.GetBufferedData();//her bir hareketin datası buffered = tamponlu, korumak

                foreach (var state in datas)
                {
                    Debug.WriteLine(state);
                    //Console.WriteLine(state + " --> " + state.Value + " --> " + state.Offset);
                    this.Invoke((MethodInvoker)delegate
                    {
                        range = state.Value / 256;


                        if (state.Offset.ToString() == "RotationX")   // Sağ analog
                        {
                            rightAndLeft = (UInt16)((state.Value) / 256);
                            Debug.WriteLine("rightAndLeft" + rightAndLeft.ToString());
                            trackBarRx.Value = state.Value;
                        }
                        else if (state.Offset.ToString() == "RotationY")   // Sağ analog
                        {
                            upAndDown = (UInt16)(255 - (state.Value) / 256);
                            Debug.WriteLine("upAndDown" + upAndDown.ToString());
                            trackBarRy.Value = -1 * state.Value;
                        }
                        else if (state.Offset.ToString() == "Y")   // Sol analog
                        {
                            motorPower = (UInt16)(255 - (state.Value / 256));
                            Debug.WriteLine("MotorPower" + motorPower.ToString());
                            trackBarLy.Value = -1 * state.Value;
                        }
                        else if (state.Offset.ToString() == "Buttons4")        //L1 butonu
                        {
                            if (state.Value == 128)
                            {
                                specialMotorMovement = true;     //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır 
                                solaDonsunMu = 1; //Sol
                                Debug.WriteLine("solBool");
                            }
                            else
                            {
                                specialMotorMovement = false;     //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır 
                                solaDonsunMu = 0; //Sag
                            }

                        }
                        else if (state.Offset.ToString() == "Buttons5")       //R1 butonu
                        {
                            if (state.Value == 128)
                            {
                                specialMotorMovement = true;      //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır 
                                sagaDonsunMu = 1; //Sag
                                Debug.WriteLine("SagBool");
                            }
                            else
                            {
                                specialMotorMovement = false;     //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır
                                sagaDonsunMu = 0;
                            }
                        }
                        else if (state.Offset.ToString() == "Buttons0")       //Yeşil A butonu    sabit kal
                        {
                            if (state.Value == 128)
                            {
                                Debug.WriteLine("YesilA");
                                sabitKalsinMi = 1; //Sabit
                                specialMotorMovement = true;      //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır 
                            }
                            else
                            {
                                sabitKalsinMi = 0;
                                specialMotorMovement = false;     //Eğer sağ ya da sola dönme isteniyorsa bu değişken 1 yapılır
                            }
                        }
                    });
                }
                                                                
                data_to_send[2] = (byte)motorPower;
                data_to_send[3] = (byte)rightAndLeft;
                data_to_send[4] = (byte)upAndDown;
                data_to_send[5] = (byte)solaDonsunMu;
                data_to_send[6] = (byte)sagaDonsunMu;
                data_to_send[7] = (byte)sabitKalsinMi;

                //8 bit CRC hesaplanır
                byte crc = Crc8.ComputeChecksum(data_to_send[0], data_to_send[1], data_to_send[2], data_to_send[3], data_to_send[4], data_to_send[5], data_to_send[6], data_to_send[7]);
                //byte check = Crc8.ComputeChecksum(1, 2, 3, crc); 
                data_to_send[8] = (byte)crc;

                dataLengthToSend = 9;

                //Datayı da yolla
                if (sendingFlag)                                                
                    com1.Write(data_to_send, 0, dataLengthToSend);
                Thread.Sleep(Kontrol.ms);
            }
        }


        private void startBtn_Click(object sender, EventArgs e)
        {
            Thread th = new Thread(uartConnection);

            if (isDroneConnected == false)    //Connect
            {
                if ((isJoystickConnected) && (isPortOpen))
                {
                    //çalış  
                    th.Start();
                    Kontrol.stop_bool = false;
                    isDroneConnected = true;
                    startBtn.Text = "Disconnect";
                }
                else
                {
                    th.Abort();
                    Kontrol.stop_bool = true;
                    isDroneConnected = false;
                    MessageBox.Show("Joystick ya da port eksik");
                    startBtn.Text = "Connect to Drone";
                }
            }
            else //Disconnect
            {
                startBtn.Text = "Connect to Drone";
                Kontrol.stop_bool = true;
                isDroneConnected = false;
            }

        }

        private void connect_joystick_btn_Click(object sender, EventArgs e)
        {
            if (isJoystickConnected == false)
            {
                isJoystickConnected = controller.Setup();
                if (isJoystickConnected)
                {
                    joystickConnLabel.Text = "Joystick Connected";
                    joystickNameLabel.Visible = true;
                    joystickNameLabel.Text = controller.getName();
                    connect_joystick_btn.Text = "Disconnect";
                }
                else
                {
                    joystickConnLabel.Text = "Joystick Not Connected";
                    joystickNameLabel.Visible = false;
                    connect_joystick_btn.Text = "Connect";
                }
            }
            else
            {
                isJoystickConnected = controller.Disconnect();
                joystickConnLabel.Text = "Joystick Not Connected";
                joystickNameLabel.Visible = false;
                connect_joystick_btn.Text = "Connect";
            }
        }

        private void stopSendingBtn_Click(object sender, EventArgs e)
        {
            if (!sendingFlag)
            {
                sendingFlag = true;
                stopSendingBtn.Text = "Stop Sending";
            }
            else
            {
                sendingFlag = false;
                stopSendingBtn.Text = "Start Sending";

            }
        }
    }

}

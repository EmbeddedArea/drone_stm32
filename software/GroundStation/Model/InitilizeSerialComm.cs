using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace GroundStation
{
    class InitilizeSerialComm : SerialPort
    {
        private Boolean IsReceiving = false;
        private Boolean IsRuning = false;
        private Boolean IsPeriodSending = false;
        private int sleepTime = 200;
        Thread receivingThread = null;
        public string yazi;
        Form1 form;

        /// <summary>
        /// Initializes a new instance of the <see cref="InitilizeSerialComm"/> class.
        /// </summary>
        /// <param name="form">The form.</param>
        public InitilizeSerialComm(Form1 form)
        {                 
            this.form = form;
            base.WriteTimeout = 50;
            //base.= new SerialPort();
            receivingThread = new Thread(DoReceive);
            IsRuning = true;
            receivingThread.Start();
            yazi = "engin";
        }

        /// <summary>
        /// Gets or sets the sleep time.
        /// </summary>
        /// <value>
        /// The sleep time.
        /// </value>
        public int SleepTime
        {
           get
           {
               return sleepTime;
           }
           set
           {
               sleepTime = value;
           }
        }
        /// <summary>
        /// This is the function which is called in "receivingThread"
        /// </summary>
        private void DoReceive()
        {
            Byte[] buffer = new Byte[1024];
            while (IsRuning)
            {
                while (IsReceiving)
                {
                    try
                    {
                        if (base.BytesToRead > 0)
                        {

                            Int32 length = base.Read(buffer, 0, buffer.Length);
                            Array.Resize(ref buffer, length);
                            UpdateForm u = new UpdateForm(form.UpdateText);
                            form.Invoke(u, new Object[] { buffer });

                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Receving thread error: " + ex.Message ,"Warning!");
                        IsReceiving = false;
                    }

                    Thread.Sleep(500);
                }
                while (!IsReceiving) Thread.Sleep(10);
            }
        }
        /// <summary>
        /// This is the function which is called in receiving thread(field name: "receivingThread")
        /// #This function is uncomplete.
        /// </summary>
        /// <param name="data">The data.</param>
        private void DoPeriodWrite(Object data)
        {
            while (true)
            {

                Byte[] dataBytes = data as Byte[];
                try
                {
                    base.Write(dataBytes, 0, dataBytes.Length);
                }
                catch (Exception e)
                {
                    //show something exception details in dialog
                    MessageBox.Show("Writing thread error: " + e.Message, "Warning!");
                }
                Thread.Sleep(sleepTime);
            }
        }
        /// <summary>
        /// This is the function which is called in writing thread(field name: "writingThread")
        /// </summary>
        /// <param name="data">this object was sent by Thread.start method</param>
        private void DoWrite(Object data)
        {
                Byte[] dataBytes = data as Byte[];
                try
                {
                    base.Write(dataBytes, 0, dataBytes.Length);
                }
                catch (Exception e)
                {
                    //show something exception details in dialog
                    MessageBox.Show("Writing thread error: " + e.Message, "Warning!");
                }                      

        }
        /// <summary>
        /// Writes the specified MSG. 
        /// </summary>
        /// <param name="msg"></param>
        new public void Write(string msg)
        {                          
            Byte[] dataBytes = Encoding.ASCII.GetBytes(msg);
            Object o = dataBytes;
            Thread writingThread = new Thread(DoWrite);
            writingThread.Start(o);
            writingThread.IsBackground = true;

        }

        /// <summary>
        /// Sets the port.
        /// </summary>
        /// <param name="portName">Name of the port.</param>
        /// <param name="baudRate">The baud rate.</param>
        public void SetPort(string portName, int baudRate)
        {
            //com1.text
            try
            {
                base.PortName = portName;
                base.BaudRate = baudRate;
            }
            catch (Exception e)
            {
                //show something exception details in dialog
                MessageBox.Show("Set port error: " + e.Message, "Warning!"); ;
            }
            base.DataBits = 8;
            base.Parity = System.IO.Ports.Parity.None;
            base.StopBits = System.IO.Ports.StopBits.One;
            base.Encoding = Encoding.ASCII;
            
        }
        /// <summary>
        /// Open port.
        /// </summary>
        /// <PermissionSet>
        ///   <IPermission class="System.Security.Permissions.SecurityPermission, mscorlib, Version=2.0.3600.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" version="1" Flags="UnmanagedCode, ControlEvidence" />
        ///   </PermissionSet>
        new public void Open()
        {
            try
            {
                base.Open();
                Thread.Sleep(200);
                    
            }
            catch (Exception e)
            {
                MessageBox.Show("Open port error: " + e.Message,"Warning!"); ;
            }
  
            IsReceiving = true;
            Thread.Sleep(200);
        }
        /// <summary>
        /// close port，let <see cref="P:System.IO.Ports.SerialPort.IsOpen" /> to false
        /// </summary>
        /// <PermissionSet>
        ///   <IPermission class="System.Security.Permissions.SecurityPermission, mscorlib, Version=2.0.3600.0, Culture=neutral, PublicKeyToken=b77a5c561934e089" version="1" Flags="UnmanagedCode, ControlEvidence" />
        ///   </PermissionSet>
        new public void Close()
        {
            try
            {
                if (base.IsOpen == true)
                {
                    IsReceiving = false;
                    
                    base.Close();
                    
                }
                
                Thread.Sleep(200);

            }
            catch (Exception e)
            {
                MessageBox.Show("Closing comport error: " + e.Message, "warning!"); ;
            }

            
        }
        /// <summary>
        /// Terminates this instance. This action will terminate receving thread and close the specific port.
        /// </summary>
        public void Terminate()
        {
            IsRuning = false;
            Close();
            receivingThread.Abort();
            receivingThread.Join();

        }
    }
}

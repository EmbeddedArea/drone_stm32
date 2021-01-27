namespace GroundStation
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.stopSendingBtn = new System.Windows.Forms.Button();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.startBtn = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.msNumber = new System.Windows.Forms.NumericUpDown();
            this.msayarla = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.connect_joystick_btn = new System.Windows.Forms.Button();
            this.joystickNameLabel = new System.Windows.Forms.Label();
            this.joystickConnLabel = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.DroneConsoleMsgDisplayRx = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.DroneConsoleMsgDisplayLy = new System.Windows.Forms.TextBox();
            this.label11 = new System.Windows.Forms.Label();
            this.DroneConsoleMsgDisplayLx = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.DroneConsoleMsgDisplayRy = new System.Windows.Forms.TextBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.portNumberComboBox = new System.Windows.Forms.ComboBox();
            this.openPortButton = new System.Windows.Forms.Button();
            this.baudRateComboBox = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.btnExit = new System.Windows.Forms.Button();
            this.clearButton = new System.Windows.Forms.Button();
            this.trackBarRx = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.trackBarRy = new System.Windows.Forms.TrackBar();
            this.trackBarLy = new System.Windows.Forms.TrackBar();
            this.listView1 = new System.Windows.Forms.ListView();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.groupBox2.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.msNumber)).BeginInit();
            this.groupBox6.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRx)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRy)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarLy)).BeginInit();
            this.groupBox7.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(28, 1038);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(152, 58);
            this.button1.TabIndex = 6;
            this.button1.Text = "Close";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.stopSendingBtn);
            this.groupBox2.Controls.Add(this.groupBox8);
            this.groupBox2.Controls.Add(this.groupBox6);
            this.groupBox2.Controls.Add(this.groupBox4);
            this.groupBox2.Controls.Add(this.groupBox5);
            this.groupBox2.Location = new System.Drawing.Point(14, 25);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox2.Size = new System.Drawing.Size(570, 698);
            this.groupBox2.TabIndex = 21;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Serial Communication LORA";
            // 
            // stopSendingBtn
            // 
            this.stopSendingBtn.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.stopSendingBtn.Location = new System.Drawing.Point(351, 623);
            this.stopSendingBtn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.stopSendingBtn.Name = "stopSendingBtn";
            this.stopSendingBtn.Size = new System.Drawing.Size(158, 54);
            this.stopSendingBtn.TabIndex = 46;
            this.stopSendingBtn.Text = "Start Sending";
            this.stopSendingBtn.UseVisualStyleBackColor = false;
            this.stopSendingBtn.Click += new System.EventHandler(this.stopSendingBtn_Click);
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.startBtn);
            this.groupBox8.Controls.Add(this.groupBox3);
            this.groupBox8.Location = new System.Drawing.Point(8, 466);
            this.groupBox8.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox8.Size = new System.Drawing.Size(544, 148);
            this.groupBox8.TabIndex = 45;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Drone Connection";
            // 
            // startBtn
            // 
            this.startBtn.BackColor = System.Drawing.Color.Firebrick;
            this.startBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.startBtn.ForeColor = System.Drawing.SystemColors.Info;
            this.startBtn.Location = new System.Drawing.Point(312, 37);
            this.startBtn.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(225, 95);
            this.startBtn.TabIndex = 5;
            this.startBtn.Text = "Connect to Drone";
            this.startBtn.UseVisualStyleBackColor = false;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.msNumber);
            this.groupBox3.Controls.Add(this.msayarla);
            this.groupBox3.Location = new System.Drawing.Point(10, 34);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox3.Size = new System.Drawing.Size(294, 95);
            this.groupBox3.TabIndex = 31;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Send Period";
            // 
            // msNumber
            // 
            this.msNumber.Location = new System.Drawing.Point(15, 42);
            this.msNumber.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.msNumber.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.msNumber.Name = "msNumber";
            this.msNumber.Size = new System.Drawing.Size(117, 26);
            this.msNumber.TabIndex = 46;
            this.msNumber.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            // 
            // msayarla
            // 
            this.msayarla.Location = new System.Drawing.Point(140, 29);
            this.msayarla.Margin = new System.Windows.Forms.Padding(3, 5, 3, 5);
            this.msayarla.Name = "msayarla";
            this.msayarla.Size = new System.Drawing.Size(130, 51);
            this.msayarla.TabIndex = 45;
            this.msayarla.Text = "Set milisecond";
            this.msayarla.UseVisualStyleBackColor = true;
            this.msayarla.Click += new System.EventHandler(this.msayarla_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.connect_joystick_btn);
            this.groupBox6.Controls.Add(this.joystickNameLabel);
            this.groupBox6.Controls.Add(this.joystickConnLabel);
            this.groupBox6.Location = new System.Drawing.Point(8, 203);
            this.groupBox6.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox6.Size = new System.Drawing.Size(544, 123);
            this.groupBox6.TabIndex = 44;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Joystick Settings";
            // 
            // connect_joystick_btn
            // 
            this.connect_joystick_btn.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.connect_joystick_btn.Location = new System.Drawing.Point(300, 29);
            this.connect_joystick_btn.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.connect_joystick_btn.Name = "connect_joystick_btn";
            this.connect_joystick_btn.Size = new System.Drawing.Size(201, 66);
            this.connect_joystick_btn.TabIndex = 1;
            this.connect_joystick_btn.Text = "Connect";
            this.connect_joystick_btn.UseVisualStyleBackColor = false;
            this.connect_joystick_btn.Click += new System.EventHandler(this.connect_joystick_btn_Click);
            // 
            // joystickNameLabel
            // 
            this.joystickNameLabel.AutoSize = true;
            this.joystickNameLabel.Location = new System.Drawing.Point(38, 45);
            this.joystickNameLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.joystickNameLabel.Name = "joystickNameLabel";
            this.joystickNameLabel.Size = new System.Drawing.Size(111, 20);
            this.joystickNameLabel.TabIndex = 0;
            this.joystickNameLabel.Text = "Joystick Name";
            // 
            // joystickConnLabel
            // 
            this.joystickConnLabel.AutoSize = true;
            this.joystickConnLabel.Location = new System.Drawing.Point(38, 75);
            this.joystickConnLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.joystickConnLabel.Name = "joystickConnLabel";
            this.joystickConnLabel.Size = new System.Drawing.Size(176, 20);
            this.joystickConnLabel.TabIndex = 0;
            this.joystickConnLabel.Text = "Joystick Not Connected";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.DroneConsoleMsgDisplayRx);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.DroneConsoleMsgDisplayLy);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.DroneConsoleMsgDisplayLx);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.DroneConsoleMsgDisplayRy);
            this.groupBox4.Location = new System.Drawing.Point(8, 349);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox4.Size = new System.Drawing.Size(544, 108);
            this.groupBox4.TabIndex = 42;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Controller Values";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label3.Location = new System.Drawing.Point(170, 25);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(23, 22);
            this.label3.TabIndex = 41;
            this.label3.Text = "Y";
            // 
            // DroneConsoleMsgDisplayRx
            // 
            this.DroneConsoleMsgDisplayRx.BackColor = System.Drawing.SystemColors.WindowText;
            this.DroneConsoleMsgDisplayRx.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DroneConsoleMsgDisplayRx.Cursor = System.Windows.Forms.Cursors.Cross;
            this.DroneConsoleMsgDisplayRx.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DroneConsoleMsgDisplayRx.ForeColor = System.Drawing.Color.LimeGreen;
            this.DroneConsoleMsgDisplayRx.Location = new System.Drawing.Point(14, 52);
            this.DroneConsoleMsgDisplayRx.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DroneConsoleMsgDisplayRx.Multiline = true;
            this.DroneConsoleMsgDisplayRx.Name = "DroneConsoleMsgDisplayRx";
            this.DroneConsoleMsgDisplayRx.ReadOnly = true;
            this.DroneConsoleMsgDisplayRx.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.DroneConsoleMsgDisplayRx.Size = new System.Drawing.Size(90, 48);
            this.DroneConsoleMsgDisplayRx.TabIndex = 34;
            this.DroneConsoleMsgDisplayRx.Text = "X";
            this.DroneConsoleMsgDisplayRx.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label12.Location = new System.Drawing.Point(40, 25);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(23, 22);
            this.label12.TabIndex = 40;
            this.label12.Text = "X";
            // 
            // DroneConsoleMsgDisplayLy
            // 
            this.DroneConsoleMsgDisplayLy.BackColor = System.Drawing.SystemColors.WindowText;
            this.DroneConsoleMsgDisplayLy.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DroneConsoleMsgDisplayLy.Cursor = System.Windows.Forms.Cursors.Cross;
            this.DroneConsoleMsgDisplayLy.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DroneConsoleMsgDisplayLy.ForeColor = System.Drawing.Color.LimeGreen;
            this.DroneConsoleMsgDisplayLy.Location = new System.Drawing.Point(132, 52);
            this.DroneConsoleMsgDisplayLy.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DroneConsoleMsgDisplayLy.Multiline = true;
            this.DroneConsoleMsgDisplayLy.Name = "DroneConsoleMsgDisplayLy";
            this.DroneConsoleMsgDisplayLy.ReadOnly = true;
            this.DroneConsoleMsgDisplayLy.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.DroneConsoleMsgDisplayLy.Size = new System.Drawing.Size(102, 48);
            this.DroneConsoleMsgDisplayLy.TabIndex = 35;
            this.DroneConsoleMsgDisplayLy.Text = "Y";
            this.DroneConsoleMsgDisplayLy.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label11.Location = new System.Drawing.Point(422, 25);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(104, 22);
            this.label11.TabIndex = 39;
            this.label11.Text = "Rotation Y";
            // 
            // DroneConsoleMsgDisplayLx
            // 
            this.DroneConsoleMsgDisplayLx.BackColor = System.Drawing.SystemColors.WindowText;
            this.DroneConsoleMsgDisplayLx.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DroneConsoleMsgDisplayLx.Cursor = System.Windows.Forms.Cursors.Cross;
            this.DroneConsoleMsgDisplayLx.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DroneConsoleMsgDisplayLx.ForeColor = System.Drawing.Color.LimeGreen;
            this.DroneConsoleMsgDisplayLx.Location = new System.Drawing.Point(268, 52);
            this.DroneConsoleMsgDisplayLx.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DroneConsoleMsgDisplayLx.Multiline = true;
            this.DroneConsoleMsgDisplayLx.Name = "DroneConsoleMsgDisplayLx";
            this.DroneConsoleMsgDisplayLx.ReadOnly = true;
            this.DroneConsoleMsgDisplayLx.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.DroneConsoleMsgDisplayLx.Size = new System.Drawing.Size(120, 48);
            this.DroneConsoleMsgDisplayLx.TabIndex = 36;
            this.DroneConsoleMsgDisplayLx.Text = "RotationX";
            this.DroneConsoleMsgDisplayLx.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(0)))));
            this.label10.Location = new System.Drawing.Point(280, 25);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(104, 22);
            this.label10.TabIndex = 38;
            this.label10.Text = "Rotation X";
            // 
            // DroneConsoleMsgDisplayRy
            // 
            this.DroneConsoleMsgDisplayRy.BackColor = System.Drawing.SystemColors.WindowText;
            this.DroneConsoleMsgDisplayRy.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DroneConsoleMsgDisplayRy.Cursor = System.Windows.Forms.Cursors.Cross;
            this.DroneConsoleMsgDisplayRy.Font = new System.Drawing.Font("Consolas", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.DroneConsoleMsgDisplayRy.ForeColor = System.Drawing.Color.LimeGreen;
            this.DroneConsoleMsgDisplayRy.Location = new System.Drawing.Point(416, 52);
            this.DroneConsoleMsgDisplayRy.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.DroneConsoleMsgDisplayRy.Multiline = true;
            this.DroneConsoleMsgDisplayRy.Name = "DroneConsoleMsgDisplayRy";
            this.DroneConsoleMsgDisplayRy.ReadOnly = true;
            this.DroneConsoleMsgDisplayRy.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.DroneConsoleMsgDisplayRy.Size = new System.Drawing.Size(114, 48);
            this.DroneConsoleMsgDisplayRy.TabIndex = 37;
            this.DroneConsoleMsgDisplayRy.Text = "RotationY";
            this.DroneConsoleMsgDisplayRy.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.portNumberComboBox);
            this.groupBox5.Controls.Add(this.openPortButton);
            this.groupBox5.Controls.Add(this.baudRateComboBox);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Location = new System.Drawing.Point(8, 42);
            this.groupBox5.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox5.Size = new System.Drawing.Size(544, 128);
            this.groupBox5.TabIndex = 43;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Comm Port Settings";
            // 
            // portNumberComboBox
            // 
            this.portNumberComboBox.FormattingEnabled = true;
            this.portNumberComboBox.Location = new System.Drawing.Point(104, 34);
            this.portNumberComboBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.portNumberComboBox.Name = "portNumberComboBox";
            this.portNumberComboBox.Size = new System.Drawing.Size(180, 28);
            this.portNumberComboBox.TabIndex = 30;
            this.portNumberComboBox.Tag = "COM1";
            this.portNumberComboBox.DropDown += new System.EventHandler(this.portNumberComboBox_DropDown_1);
            // 
            // openPortButton
            // 
            this.openPortButton.BackColor = System.Drawing.SystemColors.GradientInactiveCaption;
            this.openPortButton.Location = new System.Drawing.Point(300, 37);
            this.openPortButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.openPortButton.Name = "openPortButton";
            this.openPortButton.Size = new System.Drawing.Size(201, 71);
            this.openPortButton.TabIndex = 23;
            this.openPortButton.Text = "Open Serial Port";
            this.openPortButton.UseVisualStyleBackColor = false;
            this.openPortButton.Click += new System.EventHandler(this.openPortButton_Click);
            // 
            // baudRateComboBox
            // 
            this.baudRateComboBox.FormattingEnabled = true;
            this.baudRateComboBox.Items.AddRange(new object[] {
            "4800",
            "9600",
            "19200",
            "38400",
            "57600",
            "115200"});
            this.baudRateComboBox.Location = new System.Drawing.Point(104, 72);
            this.baudRateComboBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.baudRateComboBox.Name = "baudRateComboBox";
            this.baudRateComboBox.Size = new System.Drawing.Size(180, 28);
            this.baudRateComboBox.TabIndex = 29;
            this.baudRateComboBox.Text = "115200";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(21, 37);
            this.label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(38, 20);
            this.label9.TabIndex = 24;
            this.label9.Text = "Port";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(12, 82);
            this.label8.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(86, 20);
            this.label8.TabIndex = 25;
            this.label8.Text = "Baud Rate";
            // 
            // btnExit
            // 
            this.btnExit.BackColor = System.Drawing.Color.Firebrick;
            this.btnExit.Location = new System.Drawing.Point(1070, 651);
            this.btnExit.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(249, 54);
            this.btnExit.TabIndex = 34;
            this.btnExit.Text = "Exit Program";
            this.btnExit.UseVisualStyleBackColor = false;
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // clearButton
            // 
            this.clearButton.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.clearButton.Location = new System.Drawing.Point(28, 626);
            this.clearButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.clearButton.Name = "clearButton";
            this.clearButton.Size = new System.Drawing.Size(368, 58);
            this.clearButton.TabIndex = 33;
            this.clearButton.Text = "Clear";
            this.clearButton.UseVisualStyleBackColor = false;
            this.clearButton.Click += new System.EventHandler(this.clearButton_Click);
            // 
            // trackBarRx
            // 
            this.trackBarRx.Location = new System.Drawing.Point(116, 326);
            this.trackBarRx.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.trackBarRx.Maximum = 65536;
            this.trackBarRx.Name = "trackBarRx";
            this.trackBarRx.Size = new System.Drawing.Size(184, 69);
            this.trackBarRx.TabIndex = 15;
            this.trackBarRx.Value = 32500;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label2.Location = new System.Drawing.Point(184, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(68, 26);
            this.label2.TabIndex = 16;
            this.label2.Text = "Right";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.label1.Location = new System.Drawing.Point(46, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 26);
            this.label1.TabIndex = 12;
            this.label1.Text = "Left";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.trackBarRy);
            this.groupBox1.Controls.Add(this.trackBarLy);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.trackBarRx);
            this.groupBox1.Location = new System.Drawing.Point(1018, 26);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.groupBox1.Size = new System.Drawing.Size(314, 418);
            this.groupBox1.TabIndex = 20;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Drone Control Panel";
            // 
            // trackBarRy
            // 
            this.trackBarRy.Location = new System.Drawing.Point(188, 88);
            this.trackBarRy.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.trackBarRy.Maximum = 0;
            this.trackBarRy.Minimum = -65536;
            this.trackBarRy.Name = "trackBarRy";
            this.trackBarRy.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarRy.Size = new System.Drawing.Size(69, 235);
            this.trackBarRy.TabIndex = 21;
            this.trackBarRy.Value = -65536;
            // 
            // trackBarLy
            // 
            this.trackBarLy.Location = new System.Drawing.Point(40, 82);
            this.trackBarLy.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.trackBarLy.Maximum = 0;
            this.trackBarLy.Minimum = -65536;
            this.trackBarLy.Name = "trackBarLy";
            this.trackBarLy.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarLy.Size = new System.Drawing.Size(69, 302);
            this.trackBarLy.TabIndex = 20;
            this.trackBarLy.Value = -65536;
            // 
            // listView1
            // 
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(28, 42);
            this.listView1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(366, 570);
            this.listView1.TabIndex = 43;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.listView1);
            this.groupBox7.Controls.Add(this.clearButton);
            this.groupBox7.Location = new System.Drawing.Point(591, 25);
            this.groupBox7.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBox7.Size = new System.Drawing.Size(420, 698);
            this.groupBox7.TabIndex = 45;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Incoming Data";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.SeaShell;
            this.ClientSize = new System.Drawing.Size(1342, 735);
            this.Controls.Add(this.groupBox7);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnExit);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox8.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.msNumber)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRx)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRy)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarLy)).EndInit();
            this.groupBox7.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox portNumberComboBox;
        private System.Windows.Forms.ComboBox baudRateComboBox;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button openPortButton;
        private System.Windows.Forms.TrackBar trackBarRx;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox DroneConsoleMsgDisplayRx;
        private System.Windows.Forms.TextBox DroneConsoleMsgDisplayLy;
        private System.Windows.Forms.TextBox DroneConsoleMsgDisplayRy;
        private System.Windows.Forms.TextBox DroneConsoleMsgDisplayLx;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TrackBar trackBarRy;
        private System.Windows.Forms.TrackBar trackBarLy;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.Button clearButton;
        private System.Windows.Forms.Button msayarla;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.Button btnExit;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Label joystickConnLabel;
        private System.Windows.Forms.Button connect_joystick_btn;
        private System.Windows.Forms.Label joystickNameLabel;
        private System.Windows.Forms.NumericUpDown msNumber;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.Button stopSendingBtn;
    }
}


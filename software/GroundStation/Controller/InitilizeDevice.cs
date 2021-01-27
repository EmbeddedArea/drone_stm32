using SharpDX.DirectInput;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GroundStation.Model
{
    public class InitilizeDevice
    {
        //nuget:sharp.dx direct input

        DirectInput directInput = new DirectInput();
        Guid joystickGuid = Guid.Empty;
        public Joystick joystick;

        bool startup = false;

        public string getName()
        {
            return joystick.Properties.ProductName;
        }
        public bool Disconnect()
        {
            joystick = null;
            return false;
        }
        public bool Setup()
        {

            // Initialize DirectInput
            // Find a Joystick Guid

            foreach (var deviceInstance in directInput.GetDevices(DeviceType.Gamepad, DeviceEnumerationFlags.AllDevices))
                joystickGuid = deviceInstance.InstanceGuid;


            // If Gamepad not found, look for a Joystick
            if (joystickGuid == Guid.Empty)
                foreach (var deviceInstance in directInput.GetDevices(DeviceType.Joystick,
                        DeviceEnumerationFlags.AllDevices))
                    joystickGuid = deviceInstance.InstanceGuid;

            // If Joystick not found, throws an error
            if (joystickGuid == Guid.Empty)
            {
                Debug.WriteLine("No joystick/Gamepad found.");
            }


            // Instantiate joystick
            try
            {

                joystick = new Joystick(directInput, joystickGuid);
                // Query all suported ForceFeedback effects
                var allEffects = joystick.GetEffects();
                foreach (var effectInfo in allEffects)
                    Console.WriteLine("Effect available {0}", effectInfo.Name);

                // Set BufferSize in order to use buffered data. BufferSize = arabellek boyutu, bufferd = korumak
                joystick.Properties.BufferSize = 128;

                // Acquire the joystick , Acquire = elde etmek
                joystick.Acquire();

                startup = true;
            }
            catch (Exception ex)
            {
                return false;
                //TODO : hata mesajı controller bulunamadı
            }
            return true;
        }

    }
}

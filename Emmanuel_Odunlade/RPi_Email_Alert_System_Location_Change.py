#
# Author: Emmanuel Odunlade
# Complete Project Details http://randomnerdtutorials.com
#

import time
import serial
import string
import pynmea2
import smtplib
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText

#setting up mail information
fromaddr = "REPLACE_WITH_YOUR_EMAIL_ADDRESS"
pword = "REPLACE_WITH_YOUR_EMAIL_S_PASSWORD"
toaddr = "REPLACE_WITH_YOUR_TO_EMAIL_ADDRESS"
msg = MIMEMultipart()
msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = "Location change alert"

#setup the serial port to which GPS is connected to
port = "/dev/ttyAMA0"
ser = serial.Serial(port, baudrate=9600, timeout=0.5)
dataout  = pynmea2.NMEAStreamReader()

while True:
    newdata = ser.readline()
    print ("getting new lat")
    if newdata[0:6] == '$GPGGA':
        newmsg = pynmea2.parse(newdata)
        newlat = newmsg.latitude
        print(newlat)
        newlong = newmsg.longitude
        print(newlong)
        lat  = str(newlat)
        lon = str(newlong)
        content = "http://maps.google.com/maps?q=" + lat + "," + lon
        Email = content
        msg.attach(MIMEText(Email, 'plain'))
        try:
            server = smtplib.SMTP('smtp.gmail.com', 587)
            server.starttls()
            server.login(fromaddr, pword)
            text = msg.as_string()
            server.sendmail(fromaddr, toaddr, text)
            server.quit()
            print ("mail sent!")
        except:
            print("error, couldnt send mail, be sure to enable non secure apps login on sender's email")

        time.sleep(3)

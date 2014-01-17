import ephem  
from dateutil.tz import tzlocal
from pytz import timezone
import datetime
import time
from OSC import *   #required, to install : sudo pip install pyOSC
import subprocess
import psutil

def get_last_sunset(lat,lng):
  o=ephem.Observer()  
  o.pressure = 0
  o.horizon = '-0:34'
  #o.horizon = '-6'
  o.lat=lat
  o.long=lng
  s=ephem.Sun()  
  s.compute()  
  #print ephem.localtime(o.previous_rising(s))
  #print ephem.localtime(o.next_rising(s))
  sunset = o.previous_setting(s, use_center=True).datetime().replace(tzinfo=timezone("UTC")).astimezone(tzlocal())
  return sunset

def on():
  print "on"
  bundle = OSCBundle("/milletemps/on")
  bundle.append ("1")
  c.send(bundle)
	

def off():
  print "off"
  bundle = OSCBundle("/milletemps/off")
  bundle.append ("1")
  c.send(bundle)

def start_milletemps(pid):
  if pid > 0: 
    try:
      psutil.Process(pid).wait(1)
      return pid
    except psutil.TimeoutExpired:
      return pid
    except psutil.NoSuchProcess:
      return -1
  subp = subprocess.Popen(['/home/minad/sources/of_v0.8.0_linux64_release/apps/soixante/MilleTemps/bin/MilleTemps'])
  print "start MilleTemps"
  return subp.pid


pid = -1

while True:
  listen_address = '127.0.0.1'
  port = 6666
  c = OSCClient()
  c.connect((listen_address, port))	# connect to our OSCServer

  try:
    while True:
      pid = start_milletemps(pid)
      print "PID: " + str(pid)

      now = datetime.datetime.now().replace(tzinfo=tzlocal())
      last_sunset = get_last_sunset('43.265347','3.284921')
      poweroff = last_sunset + datetime.timedelta(1,0)
      poweroff = poweroff.replace(hour = 1)
      #print "now: " + str(now)
      #print "last sunset: " + str(last_sunset)
      #print "power off: " + str(poweroff)
      if now  > poweroff:
        off()
      else:
        on()
      time.sleep(1)
  except OSCClientError:
    print "client error"



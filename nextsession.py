import ephem  
from dateutil.tz import tzlocal
from pytz import timezone
import datetime
import time
from OSC import *   #required, to install : sudo pip install pyOSC
import subprocess
import psutil

def get_next_sunrise(lat,lng):
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
  sunset = o.next_setting(s, use_center=True).datetime().replace(tzinfo=timezone("UTC")).astimezone(tzlocal())
  return sunset

now = datetime.datetime.now().replace(tzinfo=tzlocal())
last_sunset = get_next_sunrise('43.265347','3.284921')
poweroff = last_sunset + datetime.timedelta(1,0)
poweroff = poweroff.replace(hour = 1, minute=0)
print "It is: " + str(now)
print "And the next session is starting at: " + str(last_sunset)
print "and closing at: " + str(poweroff)

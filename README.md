# throwaway-camera-upgrade
I want to upgrade a throw-away underwater camera using ESP8266.

## What do we have here?
We have here an old throw-away camera with waterproof housing which will never take a picture again, as there are no films available for it any more.

## What do we want?
Taking pictures again! Underwater! Everywhere!

## How do we fix it?
By removing the internal mechanical parts and replacing it with cheap electronics.

### Restrictions
The housing must remain waterproof, so no cutting and drilling!
The pictures must be stored internally, but must be retrieved without opening the housing.
It may happen that it falls onto the ground, so the internals should be somewhat durable.
The power source should last long.
It should be possible to check the internal state of the device.
The Wifi module of ESP8266 is one energy-hungry bastich, so it should be activated on demand.
Oh, and it should be cheap.

### Ideas
The ESP8266 provides Wifi, so we could provide a Web interface for status check and downloading/deleting the stored pictures, as well as remote software updates.

The pushbutton on the housing can be used to
- take pictures
- maybe check the internal state
- maybe activate Wifi

The wind-up wheel on the housing can be used to
- activate the device
- maybe check the internal state
- maybe activate Wifi

An additional reed switch within the housing could be used to
- activate Wifi

The internal flash is too energy-consuming, so we leave it be.

A step-up-converter can be used to provide 5V stable, driven by a 9V rechargeable battery.

The device should go to sleep after, say, 30 seconds or so.

### Parts
- throw-away camera and housing (found one for 1€ at a flea market)
- NodeMCU ESP3266 (4-7€)
- camera module (4-7€)
- SD card reader module (1-4€)
- SD card (3-6€)
- 9V rechargeable battery (2-5€)
- wires, little switch, hot glue (of course!), battery clip... (?€)

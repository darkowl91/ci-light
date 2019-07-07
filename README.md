ci-light
===
> ESP8266/Node MCU project to present ci build status via traffic light

## Background
Saw this many times in varius articles on the social impacts of making the CI status more visible and how that encourages people to fix things that are broken. One way to improve the visibility beyond just having a large traffic light displaying the status of CI server. Green means everything built successfully. Yellow means build is unstable. Red means something is broken.

## Setup
```
-----------                     -----------------------
| Jenkins | -- Build Status --> | Private TGM Channel |
-----------                     -----------------------
                                          ^
                                          |
         -------------------    -----------------------
         | 3ch Relay Board |<---| ESP8266 TGM Bot API |
         -------------------    -----------------------
```

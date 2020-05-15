# intel8080-emulator

Originally following the [Emulator-101 tutorial](http://emulator101.com/), but diverged when tutorial switched to Cocoa.

Currently _very_ WIP, working on getting the emulator to pass some diagnostic tests.

End goal is to emulate Space Invaders

## Testing 

Run `make test & ./8080testrunner`

Only the `8080PRE.COM` tests pass currently.

## Running Space Invaders

To run Space Invaders you'll need to provide a ROM, the rom generally comes in 4 parts:

```
invaders.e
invaders.f
invaders.g
invaders.h
```

Copy these files into `./rom` and build and run using `./intel8080 rom/invaders` (I'll clean this up a bit soonTM)

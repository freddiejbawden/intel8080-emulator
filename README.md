# intel8080-emulator

Originally following the [Emulator-101 tutorial](http://emulator101.com/), but diverged when tutorial switched to Cocoa.

**Goals**

- [x] Tests passing
- [x] Attract Mode Running
- [ ] SDL Taking Input
- [ ] Game Playable
- [ ] Sound
## Testing 

Run `make test && ./8080testrunner`

Tests will take around 5 minutes to run

Expected Output
```
*********************************
./tests/CPUTEST.COM loaded

DIAGNOSTICS II V1.2 - CPU TEST
COPYRIGHT (C) 1981 - SUPERSOFT ASSOCIATES

ABCDEFGHIJKLMNOPQRSTUVWXYZ
CPU IS 8080/8085
BEGIN TIMING TEST
END TIMING TEST
CPU TESTS OK

*********************************
./tests/TEST.COM loaded
MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC VERSION 1.0  (C) 1980

CPU IS OPERATIONAL
*********************************
./tests/8080PRE.COM loaded
8080 Preliminary tests complete
*********************************
./tests/8080EX1.COM loaded
8080 instruction exerciser (KR580VM80A CPU)
dad <b,d,h,sp>................  OK
aluop nn......................  OK
aluop <b,c,d,e,h,l,m,a>.......  OK
<daa,cma,stc,cmc>.............  OK
<inr,dcr> a...................  OK
<inr,dcr> b...................  OK
<inx,dcx> b...................  OK
<inr,dcr> c...................  OK
<inr,dcr> d...................  OK
<inx,dcx> d...................  OK
<inr,dcr> e...................  OK
<inr,dcr> h...................  OK
<inx,dcx> h...................  OK
<inr,dcr> l...................  OK
<inr,dcr> m...................  OK
<inx,dcx> sp..................  OK
lhld nnnn.....................  OK
shld nnnn.....................  OK
lxi <b,d,h,sp>,nnnn...........  OK
ldax <b,d>....................  OK
mvi <b,c,d,e,h,l,m,a>,nn......  OK
mov <bcdehla>,<bcdehla>.......  OK
sta nnnn / lda nnnn...........  OK
<rlc,rrc,ral,rar>.............  OK
stax <b,d>....................  OK
```

## Running Space Invaders

To run Space Invaders you'll need to provide a ROM, the ROM generally comes in 4 parts:

```
invaders.e
invaders.f
invaders.g
invaders.h
```

Copy these files into `./rom` and build and run using `./intel8080 rom/invaders` (I'll clean this up a bit soonTM)

# gr-dect2 #

The original code was forked from [this github project][1a].

This project was developed to demonstrate the possibility of real-time DECT voice channel decoding by GnuRadio. it
allows to listen to a voice when encryption isn't applied. As an example DECT digital baby monitors don't perform
encryption. 

**WARNING:** _You take **full responsibility** when using this software. It is intended for demonstration purposes
**ONLY** and use of it for eavesdropping on phone calls **is illegal in most countries**; consult your local laws._


## Changes in my Fork ##

I've added the US DECT _(sometimes referred to as **DECT 6.0** devices)_ channels (among others) to enable the use of
the **1.9 GHz band** with the software.

I'm also working to _(hopefully)_ implement the ability to select **RFP** or **PP** as your part, rather than running
all over the place or perhaps scanning all parts automatically.

Finally, I'd like to add a scan function, so that you don't need to manually go through everything.


### Shoutouts ###

Shoutout to the original developer, [Pavelyazev][1], for whom without this would be impossible to do on an SDR. We're
getting close to something that is comparable to the **Com-on-air card** for monitoring and I find it very exciting.


## Hardware Requirements ##

DECT operates in the **1880–1900 MHz band** and occupies ten channels from **1881.792 MHz** to **1897.344 MHz**. So, in
order to receive a DECT digital stream, an appropriate SDR hardware device is necessary. This project was developed and
tested with the Ettus **USRP2 + WBX daughterboard** and **USRP B200**. 

It should also work with other SDR devices that can cover the **1880–1900 MHz band** and can provide an effective sample
rate that's at least **double** the DECT data rate _(**1,152,000 bps** or **1.152 MS/s**)_.
_Note, however, that some adaptation may be necessary._

As a DECT link **source**, the **Motorola MBP12** baby monitor was used.

Because of the high DECT data rate, a computer on which to run the project should be powerful enough. 


## Compilation Instructions ##

To build the `gr-dect2` [GnuRadio][2] module, execute the following commands in a terminal window:

```
git clone https://github.com/SignalsEverywhere/gr-dect2.git
cd gr-dect2/
mkdir build
cd build
cmake ../
make
sudo make install
sudo ldconfig
```

Then **GnuRadio Companion** should be used to open and run the flow graph `dect2.grc` from `gr-dect/grc/`


## Usage ##

Each device that can emit a DECT signal will be called a _"part"_. According to the DECT specification, there are two
types of parts -– **RFP _(Radio Fixed Part)_** or _base station_ and **PP _(Portable Part)_** or _handset_. The **RFP**
emits a signal that sets up a frame structure on the air. An **RFP** can be listened to independently. But, in order to
get a voice from a **PP**, it is necessary to receive its pair **RFP**.

The project uses _**QT**-based_ controls. There are:
+ RX gain slider
+ channel drop-down list
+ receiver ID drop-down list
+ status console

The status console shows parts on the air. Information about a part consists of a receiver ID, the part’s ID in the DECT
system, the part’s type and a voice presence sign. The status is updated every time when a part is gained/lost or voice
data is gained/lost. A pair of **RFP** and **PP** will have the same DECT ID.

The receiver ID is an internally assigned number inside the receiver. The current implementation allows to listen to
only one part. A necessary part is selected by ID from the drop-down list. The selected part will be marked by an
**asterisk** (`*`) in the console. If voice data is available, a status line will have the `v` letter at the end, and
decoded voice will be routed to a sound card.

From time to time, parts may change their **frequency** _(channel)_. So, to catch something, a periodic manual scan over
channels is necessary.


## OS-specific Notes ##


### GnuRadio Installation on Ubuntu 16.04 LTS ###

```
Sudo apt-get update
sudo apt-get install cmake gnuradio gr-osmosdr swig git volk-profile
```




[1]:	<https://github.com/pavelyazev/>
[1a]:	<https://github.com/pavelyazev/gr-dect2>
[2]:	>https://www.gnuradio.org>


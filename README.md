# Kong

Various software/firmware for Kong, a small, autonomous vehicle that monitors elopement. Kong is comprised of a vanilla [Donkey Car][donkeycar] platform with the addition of a [Redbear Duo][redbear] that handles PWM input and Bluetooth scanning.

Because Kong is a rover, multiple components require software of different flavors and languages.

- `bin` includes a set of scripts for managing the rover over SSH.
- `config` includes configuration files for the onboard [Raspberry Pi][raspberrypi].
- `car` includes the "car" project created and managed by the [donkeycar][donkeycar-py] project.
- `duo` includes the firmware code for the onboard [Redbear Duo][redbear].
- The `Makefile` at the project's root includes targets for building and flashing the Duo code over Serial and training a &ldquo;sample&rdquo; model from training data in a similarly named tub.

[donkeycar]: http://www.donkeycar.com/
[donkeycar-py]: https://github.com/wroscoe/donkey
[raspberrypi]: https://www.raspberrypi.org/
[redbear]: https://redbear.cc/duo

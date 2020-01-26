# Info #
This repository contains a custom firmware for the ST CR95HF demo board, which is (also) part of the [M24LR-DISCOVERY](https://www.st.com/en/evaluation-tools/m24lr-discovery.html) kit.

# Differences #
Original firmware has been slightly modified to allow the following behaviour to be "atomic":
1. Turn field off
2. Wait user-controllable amount of time `Time1`
3. Turn field back on
4. Wait user-controllable amount of time `Time2`
5. Send NFC command to the tag & wait for response

Previously these actions could be performed controlling the CR95HF front end directly from the PC via HID commands, but it was not possible to have precise timing control.

Introduction of atomic commands allows to send an HID packet to the board which contains `Time1`, `Time2` and the NFC command. Appropriate waits are performed on the STM32 MCU onboard, delivering repeatability of results.

Atomic commands are formatted like this: `cmd[] = {0x03, T1, T1, T2, T2, 0x04, cmd_length, cmd...}` where T1 and T2 are two 16 right aligned bits integers which represent wait time in microseconds (uS).

# TODO #
As of now the FieldOn command is hardcoded in the firmware, it should be trivial (and should be done before publication) to make it user-configurable. By user-configurable I mean that the configuration should be transparent to the user, as the last used configuration will be saved by the firmware in its ram in order to restore the CR95HF to the chosen config, whatever it may be.

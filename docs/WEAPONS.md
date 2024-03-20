# Weapons

This page will cover how the game stores each weapon in a save file. There are a few things to note to get started:

- Weapons are stored linearly from offset `0x670`.
- Weapons are stored in sections of 32 bytes, though most is empty space.
- Data related to weapon ID is stored at weapon offset + `0x05`.
- Weapons are listed from most recently obtained and onwards.

# IDs

Weapons contain two different types of IDs. Those being their class ID, and weapon ID.

Class IDs are from 0 to 8 and are as follows:

0. Blades
1. Staves
2. Claws
3. Bows
4. Palms                                 
5. Clubs
6. Cannons
7. Orbitars
8. Arms

Weapon IDs are from 0 to 11, and are specific to each type of weapon. A quick example of this would be the weapon with class ID of zero, and weapon ID of zero. This is the first blade. Below in the document are all weapons with their ID.

# How IDs Are Stored

Weapons are stored in 32 byte segments, most of which is padded by zeroes. We are concerned with two bytes in particular, stored at the weapon's offset + `0x05`. Within these two bytes there are three pieces of information:

- The class ID.
- The weapon ID.
- A partial representation of the time the weapon was obtained.

We only care about the first two.

The bitfields are laid out as follows:

| Bit 0  | Bit 1  | Bit 2 - 5 | Bit 6 - 12 | Bit 13 - 15 |
| ------ | ------ | --------- | ---------- | ----------- |
| wid[4] | ignore | Class ID  |   ignore   |   wid[0:3]  |

Or put more plainly, 

```
Class ID:
0000 0000 0000 0000
  -- --

Weapon ID:
0000 0000 0000 0000
-               ---
Where the first bit is appended to the last three.
```

### Example

Consider the following weapon:
```
00 00 00 00 00 0C 83 11 05 00 00 00 00 00 00 00
00 00 00 00 83 00 B3 00 52 00 15 00 D9 00 00 00
```

We are concerned with bytes `0x0C` and `0x83`. Converted to binary we have `0000 1100 1000 0011`.

The bits that fall under the class ID are `0011` which is 3. This makes this a bow. The bits that fall under the weapon ID are `0` and `011`. Appending the former to the latter, we get `0110` which is 8. This is a Sagittarius Bow.

With that and the list below, we have all the information we need to edit weapon types.

# Weapon list

A list of weapons I have put together with their respective IDs.

### Blades

0. First Blade.
1. Burst Blade.
2. Viper Blade.
3. Crusader Blade.
4. Royal Blade. 
5. Optical Blade.
6. Samurai Blade.
7. Bullet Blade.
8. Aquarius Blade.
9. Aurum Blade.
10. Palutena Blade.
11. Gaol Blade.

### Staves

0. Insight Staff.
1. Orb Staff.
2. Rose Staff.
3. Knuckle Staff.
4. Ancient Staff.
5. Lancer Staff.
6. Flintlock Staff.
7. Somewhat Staff.
8. Scorpio Staff.
9. Laser Staff.
10. Dark Pit Staff.
11. Thanatos Staff.

### Claws

0. Tiger Claws.
1. Wolf Claws.
2. Bear Claws.
3. Brawler Claws.
4. Stealth Claws.
5. Hedgehog Claws.
6. Raptor Claws.
7. Artillery Claws.
8. Cancer Claws.
9. Beam Claws.
10. Viridi Claws.
11. Pandora Claws.

### Bows

0. Fortune Bow.
1. Silver Bow.
2. Meteor Bow.
3. Divine Bow.
4. Darkness Bow.
5. Crystal Bow.
6. Angel Bow.
7. Hawkeye Bow.
8. Sagittarius Bow.
9. Aurum Bow.
10. Palutena Bow.
11. Phosphora Bow.

### Palms

0. Violet Palm
1. Burning Palm
2. Needle Palm.
3. Midnight Palm.
4. Cursed Palm.
5. Cutter Palm.
6. Pudgy Palm.
7. Ninja Palm.
8. Virgo Palm.
9. Aurum Palm.
10. Viridi Palm.
11. Great Reaper Palm.

### Clubs

0. Ore Club.
1. Babel Club.
2. Skyscraper Club.
3. Atlas Club.
4. Earthmaul Club.
5. Ogre Club.
6. Halo Club.
7. Black Club.
8. Capricorn Club.
9. Aurum Club.
10. Hewdraw Club.
11. Magnus Club.

### Cannons

0. EZ Cannon.
1. Ball Cannon.
2. Predator Cannon.
3. Poseidon Cannon.
4. Fireworks Cannon.
5. Rail Cannon.
6. Dynamo Cannon.
7. Doom Cannon.
8. Leo Cannon.
9. Sonic Cannon.
10. Twinbellows Cannon.
11. Cragalanche Cannon.

### Orbitars

0. Standard Orbitars.
1. Guardian Orbitars. 
2. Shock Orbitars. 
3. Eyetrack Orbitars.
4. Fairy Orbitars.
5. Paw Pad Orbitars.
6. Jetstream Orbitars.
7. Boom Orbitars.
8. Gemini Orbitars.
9. Aurum Orbitars.
10. Centurion Orbitars.
11. Arlon Orbitars.

### Arms

0. Crusher Arm.
1. Compact Arm.
2. Electroshock Arm.
3. Volcano Arm.
4. Drill Arm.
5. Bomber Arm.
6. Bowl Arm.
7. End-All Arm.
8. Taurus Arm.
9. Upperdash Arm.
10. Kraken Arm.
11. Phoenix Arm.


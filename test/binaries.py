#!/usr/bin/env python3
"""
Author: Schuy
Script to generate binary test files.
"""
import os

def get_weapon_bin(name: str, stars: list[str], mods: list[str]) -> bytes:
    """
    Generates a binary object in this format:

    ```
    00 00 00 00 00 aa aa 00 bb 00 cc 00 00 00 00 00
    00 00 00 00 ff 00 ff 00 ff 00 ff 00 ff 00 ff 00
    ```

    Where:
    `aaaa` represents the name.
    `bb` represents ranged stars.
    `cc` represents melee stars.
    `ff` each represent one modifier value.

    If confused on how values are represented in binary, please see WEAPONS.md
    within the docs directory, or browse the following gbatemp forum:

    https://gbatemp.net/threads/kid-icarus-uprising-weapons-save-hex-editing.380354/

    Parameters:
    -----------
    `name: str`
        String representation of two bytes in hex for the name.
    `stars: list[str]`
        Two strings representing a byte each, one for each star value.
    `mods: list[str]`
        A list of mods ranging from 0 - 6 in len, each representing a mod.

    Returns:
    --------
    `bin_data: bytes`
        Binary object representing weapon.
    """
    bin_data = bytes.fromhex("0000000000")
    bin_data += bytes.fromhex(name) + bytes.fromhex("00")
    bin_data += bytes.fromhex(stars[0] + "00" + stars[1])
    bin_data += bytes.fromhex("000000000000000000")
    for mod in mods:
        bin_data += bytes.fromhex(mod) + bytes.fromhex("00")
    while (len(bin_data) != 32):
        bin_data += bytes.fromhex("00")
    return bin_data


def generate_bin_file(path: str, name: str, stars: list[str], mods: list[str]) -> None:
    """
    Calls `get_weapon_bin` and writes to a binary file.

    Parameters:
    -----------
    `path: str`
        Path to file being written.
    `name: str`
        String representation of two bytes in hex for the name.
    `stars: list[str]`
        Two strings representing a byte each, one for each star value.
    `mods: list[str]`
        A list of mods ranging from 0 - 6 in len, each representing a mod.
    """
    bin_data = get_weapon_bin(name, stars, mods)
    with open(path, "wb") as file:
        file.write(bin_data)

def generate_n_weapons(path: str, name: str, stars: list[str], mods: list[str], n: int) -> None:
    """
    Generates n weapons by calling `get_weapon_bin` n times. Writes to a 
    binary file.

    Parameters:
    -----------
    `path: str`
        Path to file being written.
    `name: str`
        String representation of two bytes in hex for the name.
    `stars: list[str]`
        Two strings representing a byte each, one for each star value.
    `mods: list[str]`
        A list of mods ranging from 0 - 6 in len, each representing a mod.
    `n: int`
        Number of weapons being written.
    """
    bin_data = b''
    for _ in range(n):
        bin_data += get_weapon_bin(name, stars, mods)

    # Create 32 byte sentinel values if n is 999, as 999 is max weapons and
    # KIUEdit should stop reading after 999 values reached.
    if n == 999:
        for _ in range(32):
            bin_data += bytes.fromhex("ff")

    # Otherwise, simply create an empty weapon in order to stop reading.
    else:
        for _ in range(32):
            bin_data += bytes.fromhex("00")

    with open(path, "wb") as file:
        file.write(bin_data)


if __name__ == "__main__":
    # If file dir doesn't exist, create it.
    if not os.path.exists("test/input/file/"):
        os.mkdir("test/input/file/")

    # Generate an empty weapon file.
    prefix = "test/input/file/"
    generate_bin_file(f"{prefix}empty.bin", "0000", ["00", "00"], [])

    # Generate a First Blade with 6 ranged stars.
    generate_bin_file(f"{prefix}first_blade_6r.bin", "0000", ["0c", "00"], [])

    # Generate a Gaol Blade with 6 ranged and 6 melee stars.
    generate_bin_file(f"{prefix}gaol_blade_6r6m.bin", "8005", ["0c", "1c"], [])

    # Generate a Drill Arm with 6 melee stars.
    generate_bin_file(f"{prefix}drill_arm_6m.bin", "2002", ["19", "00"], [])

    # Generate a cookie cutter Drill Arm.
    generate_bin_file(
        f"{prefix}drill_arm_cookie.bin",
        "2002",
        ["09", "00"],
        ["b5", "bb", "66", "0a", "92", "8a"]
    )

    # Generate a file with non-random weapon info.
    # Would write a function for this, but this is simply a unique test case
    # that is only generated once. Other tests will just repeatedly output
    # the same weapon.

    # NOTE: Could write functions that could randomly create weapons.
    weapon1 = ("9c21", ["09", "00"], ["b5", "bb", "66", "0a", "92", "8a"])
    weapon2 = ("0c20", ["0c", "00"], ["b5", "bb", "65", "0a", "8a"])
    weapon3 = ("1801", ["02", "12"], ["4f", "16", "31"])
    weapon4 = ("2023", ["0c", "00"], ["b5", "bb", "65", "0a", "92", "8a"])
    weapon5 = ("0802", ["03", "12"], ["08", "26"])
    weapon6 = ("1c03", ["0c", "00"], ["b5", "52", "66", "0a", "8a"])
    weapon7 = ("8405", ["01", "00"], ["6e", "ac", "16"])
    blank = ("0000", ["00", "00"], [])
    bin = get_weapon_bin(*weapon1)
    bin += get_weapon_bin(*weapon2)
    bin += get_weapon_bin(*weapon3)
    bin += get_weapon_bin(*weapon4)
    bin += get_weapon_bin(*weapon5)
    bin += get_weapon_bin(*weapon6)
    bin += get_weapon_bin(*weapon7)
    bin += get_weapon_bin(*blank)
    bin += get_weapon_bin(*blank)
    with open(f"{prefix}weapon_list.bin", "wb") as file:
        file.write(bin)
    

# Lab 04: Accessing SBC Remotely

This document provides the exhaustive laboratory execution protocol for **Lab 04: Accessing SBC Remotely** (Module Code: `AI53003FP`, Duration: 3 hours).

---

## 1. Laboratory Objectives

1. Understand the architecture and role of the [[Single-Board-Computer-Raspberry-Pi-Gateway|Single Board Computer (SBC)]] in fog computing.
2. Establish a headless network connection to the SBC via Wi-Fi / Ethernet.
3. Access the Linux command shell via **SSH** using **MobaXterm** on TCP Port `22`.
4. Establish a remote graphical desktop session via **VNC** using **RealVNC Viewer** on TCP Port `5900`.
5. Execute essential Linux terminal navigation and file manipulation commands.

---

## 2. Remote Access Configuration

### 1. SSH Access via MobaXterm:
1. Open MobaXterm and click **Session** > **SSH**.
2. **Remote host:** Enter the SBC's local IP address (e.g., `192.168.1.105` discovered via hotspot / network scanner).
3. **Specify username:** Check box and enter `pi`.
4. **Port:** Ensure port is set to `22`.
5. Click **OK**. When prompted, enter the password.

### 2. Graphical VNC Desktop via RealVNC Viewer:
1. Open RealVNC Viewer.
2. In the VNC Server connection bar, enter: `192.168.1.105:5900` (or simply the IP address).
3. Press Enter, accept identity encryption warnings, and enter login credentials (`pi` / password).
4. The Raspberry Pi desktop GUI appears in a responsive window.

---

## 3. Core Linux Command Reference

| Command | Description | Example Usage |
| :--- | :--- | :--- |
| `pwd` | Print working directory | `pwd` |
| `ls -la` | List all files with permissions | `ls -la` |
| `cd <dir>` | Change directory | `cd /home/pi/aiot` |
| `mkdir <dir>`| Create a new directory | `mkdir lab_scripts` |
| `nano <file>`| Terminal text editor | `nano test.py` |
| `cat <file>` | Concatenate and display file content | `cat /etc/os-release` |
| `sudo` | Superuser Do (execute with root privileges)| `sudo apt update` |
| `ip addr` / `ifconfig`| Display network interfaces and IP addresses| `ip addr show wlan0` |
| `python3 <script>`| Execute Python 3 script | `python3 01_thingSpeak_Basic.py`|

---

## Traceability & Source Excerpts

> **Source:** `03_Lab Sheets/Lab04_Accessing_SBC_Remotely.pdf` (Pages 1-8)
> 
> "A Single Board Computer (SBC) is a computer built on a single circuit board... Connect to SBC using MobaXterm for SSH terminal on port 22 and VNC Viewer for remote desktop on port 5900... Use basic Linux commands: ls, cd, mkdir, nano, sudo, ifconfig, python3."

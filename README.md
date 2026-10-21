# Project Overview

As a member of the Malware Development and Evasion team at the Dragons Student Science Club in Lodz University of Technology, I've developed a proof-of-concept (PoC) malware designed to perform local privilege escalation (LPE) from a administrator to NT AUTHORITY\SYSTEM.
In the future, the goal is to conduct post-exploitation activities. Currently, the focus is on disabling Windows Tamper Protection, including investigating techniques such as PPL bypasses that modify protection flags.

### Rootkit BYOVD
![Alt text](/screenshots/BYOVD.png?raw=true "BYOVD")

### LPE
![Alt text](/screenshots/LPE.png?raw=true "LPE")

### PPL
![Alt text](/screenshots/PPL.png?raw=true "PPL")

### CMD before and after elevation
![Alt text](/screenshots/BEFORE.png?raw=true "BEFORE")

The PID is different, bcs I closed cmd by mistake and had to do it again, but trust me it works.
First whoami after LPE, see the NT SYSTEM privileges, second after PPL my windows doesn't allow to execute programs from this flags.
![Alt text](/screenshots/AFTER.png?raw=true "AFTER")

My Role & Key Contributions

    Conducted in-depth research on Bring Your Own Vulnerable Driver (BYOVD) techniques.
    Developed a custom program leveraging the vulnerable Gio.sys driver to successfully execute the attack.

Key Learnings & Skills Acquired

    Deepened practical understanding of Windows Internals and OS architecture.
    Gained hands-on experience with the reverse engineering, mechanics, weaponization, and execution of kernel-level BYOVD attacks.

# AssemblyProject
This is a small university project developed with the collaboration of other three collegues to test the knowledge of the Assembly code. 
In particular this project is done based on the architecture of the micro processor MIPS R2000 (clock frequency of 500MHz).

## Technologies Used
To simulate the code we used the [PC SPIM](https://pages.cs.wisc.edu/~larus/SPIM)

## Explanation of the project
The project is based on the simulation of a gasoline dispenser. Specifically, the user-machine interaction is simulated by setting specific registers of the PC SPIM. For example, simulating the insertion of a 5 euro banknote by the customer was achieved by setting a particular cell of the microcomputer to 0, or the waiting time for fuel dispensing was managed using a cycle that increments a register by 1 until a certain value proportional to the wait time is reached.
In the end it is a very basic project, but the aim of this assignement was to test our knowledge of the assembly code, and to understand better the architecture behind the MIPS.

## Real Problem
The user inserts some banknotes inside the machine, then when it's ready a button is pushed. Then the customer must insert the nozzle into the tank inlet and then press the dispenser's start button. Then he must wait until the dispencer has finished

## General explanation of the code
In the [pdf file](AssemblyProject/blob/main/MIPS.pdf)  there is a flow chart diagram to explain how the code works.

The microcomputer exchanges information with the bill acceptor through the 16-bit cell M_BANC and with the dispenser through the 16-bit cell ERG. Then, it waits on the M_BANC cell until the bill acceptor notifies the presence of a banknote by setting line 15 to 1. The microcomputer sends the LEGGI (read in italian) command (coded as 4000 in hexadecimal) to the M_BANC cell and waits until the machine sets line 7 to 1, indicating that the banknote has been successfully read. Additionally, bits 3 and 2 encode the following 4 cases:
- 00: €5 banknote
- 01: €10 banknote
- 10: €20 banknote
- 11: Invalid banknote. In this case, the microcomputer sends the ESPELLI (expluse in italian) command (coded as 1000 in hexadecimal) to the M_BANC cell.
The microcomputer reads the data in the cell and updates the sum of all the banknotes. Then it waits on the M_BANC cell until the confirmation button for the amount is pressed (as said before this is simulated by setting a certain value inside a cell of the MIPS). When this event occurs, the machine sets line 4 in the M_BANC cell to 1. At this point, the microcomputer suspends bill acceptance.

The dispenser informs the microcomputer of the button press by setting line 15 in the ERG cell to 1. At this point, the microcomputer activates the dispenser by setting line 14 in the ERG cell to 1, resets the amount, and re-enables bill acceptance. We asssume that the dispensing process lasts for two and a half minutes.








	.data
M_BANC: .half 0 #cella inizializzata a 0
	.space 2 #lascia due celle vuote (altrimenti ha problemi set value)
ERG: 	.half 0 #cella inizializzata a 0
	.text
	
	la $t0,M_BANC #carica in $t0 l’indirizzo della cella M_BANC
	la $t7,ERG #carica in $t7 l’indirizzo della cella ERG
	li $t1,0x8000 #settato il valore della MASK (inserimento soldi)
	li $t2,0xC000 #settato il valore della MASK(5€)
	li $t3,0xC004 #settato il valore della MASK(10€)
	li $t4,0xC008 #settato il valore della MASK(20€)
	li $t5,0xC00C #settato il valore della MASK(non valido)
	li $t6,0xC010 #settato il valore della MASK bottone M_BANC
	li $t9,0x4000 #settato il valore della MASK (erogazione avviata)
	li $t8,0xC080 #settato il valore della MASK (linea 7)

TEST: 	sh $zero,0($t0) #pongo a 0 la M_BANC
	sh $zero,0($t7) #pongo a 0 la ERG

NEW_U: 	#settare il valore 0x8000
	lhu $s0, 0($t0) #s0 contiene il dato salvato in memoria
	and $s1,$s0,$t1 #confronta la MASK con $s0
	bne $s1,$t1,TEST #rimane nel loop se MASK!=$s1

LEGGI: 	#inserire banconota con set value (0x8000 =5€) (0x8004 =10€) (0x8008 #=20€)(0x800C=espelli)
	lhu $s0, 0($t0)
	addi $s0,$s0,0x4000 #settato LEGGI a 1
	and $s1,$s0,$t2 #controllo lettura ($t2 confronta i primi due bit =1)
	bne $s1,$t2, ESPELLI #se non è avvenuta la lettura si espelle la banconota
	sh $s0,0($t0) #aggiorno il valore di M_BANC
	beq $s0, $t2,B1 # 5€
	beq $s0, $t3,B2 #10€
	beq $s0, $t4,B3 #20€
	beq $s0, $t5,B4 #banconota errata

B1: 	addi $v0,$v0,5 #aggiungo 5€ di credito
	j LETTURA
B2: 	addi $v0,$v0,10 #aggiungo 10€ di credito
	j LETTURA
B3: 	addi $v0,$v0,20 #aggiungo 20€ di credito
	j LETTURA
B4: 	j ESPELLI

ESPELLI: addi $s0, $s0, 0x1000
	sh $s0,0($t0) #aggiorno il valore di M_BANC
	j TEST

LETTURA: addi $s0,$s0,0x0080 #settato LETTURA BANCONOTA a 1 (linea 7=1)
	and $s0,$s0,$t8
	bne $s0,$t8,TEST #linea 7=0 (verifica avvenuta lettura)

PULS_BANC: #se si vuole terminare l’aggiunta di banconote per iniziare l’erogazione, #settare(0xC010)
	lhu $s0, 0($t0) #per settare il valore del pulsante
	and $v1,$s0,$t6 #confronto per linea 4
	bne $v1, $t6, TEST #se != torna a test (no pulsante) altrimenti ERG

PULS_ERG: #se si vuole avviare l’erogazione,settare(0x8000) cella ERG
	lhu $s2, 0($t7) #carica in $s2 il contenuto dell'indirizzo $t7 (ERG)
	and $s2,$s2,$t1 #confronto per linea 15
	bne $s2,$t1,PULS_ERG #controllo pressione pulsante

AVVIO: 	or $s2,$s2,$t9 #setta la linea 14 a 1
	sh $s2,0($t7) #aggiorno il valore di ERG
	sub $v0,$v0,$v0 #azzerato l’importo totale

ATTESA: #ciclo per simulare l’attesa (ripetuto 15 volte per arrivare a 2.30 minuti)
	li $s5,0x9502F900 #setta 2.500.000.000
EROGAZ: addi $s5,$s5,-1 #ogni ciclo perde 10s
	bne $s5,$zero,EROGAZ
	addi $s3,$s3,1 #incrementato contatore
	slti $s4,$s3,14 #in $s4 viene messo il risultato della slt
	bne $s4,$zero,ATTESA #se non è a 14 (15 volte ciclo) ripete il ciclo
	j AZZERA #nuovo cliente *breakpoint

AZZERA: sub $v1,$v1,$v1 #setto $v1 a zero
	sub $s0,$s0,$s0 #setto a $s0 a zero
	sub $s1,$s1,$s1 #setto a $s1 a zero
	sub $s2,$s2,$s2 #setto a $s2 a zero
	sub $s3,$s3,$s3 #setto valore contatore =0
	sh $zero,0($t0) #pongo a 0 la M_BANC
	sh $zero,0($t7) #pongo a 0 la ERG
	j NEW_U #salta a nuovo utente
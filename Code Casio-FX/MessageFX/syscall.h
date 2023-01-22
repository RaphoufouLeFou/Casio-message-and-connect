#ifndef _SYSCALL
#define _SYSCALL

/**********************************************************/
/** Les syscall suivants servent à l'acces au port série **/
/** Le buffer de réception fait 1ko,                     **/
/** et le buffer d'envoi fait 256 octets.                **/
/**********************************************************/

//Lit un caractère du buffer de réception et le copie a l'adresse pointée par 'dest'
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas établie
int Serial_ReadByte(unsigned char *dest);

//Lit 'max' octets du buffer de réception et les écrit dans 'dest'
//'size' donne le nombre d'octets lu
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas établie
int Serial_ReadBytes(unsigned char *dest, int max, short *size);

//Ecrit 'byte' dans le buffer d'envoi
//Retourne 0
int Serial_WriteByte(unsigned char byte);

//Ecrit 'size' octets dans le buffer d'envoi depuis l'adresse 'scr'
//Retourne 0 en cas de succes, 2 si le buffer est trop plein, 3 si la connexion n'est pas établie
int Serial_WriteBytes(unsigned char *src, int size);

//FIFO = first in first out
int Serial_WriteByteFIFO(unsigned char byte);

//Retourne la taille des données du buffer de réception
int Serial_GetRxBufferSize(void);

//Retourne l'espace disponible dans le buffer d'envoi
int Serial_GetTxBufferFreeCapacity(void);

//Vide le buffer de réception
//Retourne 0 en cas de succes, 3 si la connexion n'est pas établie
int Serial_ClearReceiveBuffer(void);

//Vide le buffer d'envoi
//Retourne 0
int Serial_ClearTransmitBuffer(void);

//Ouvre et prépare l'interface de communication
//Pour plus de détails, consulter fxreverse-doc-1.pdf ci joint
int Serial_Open(unsigned char *conf);

//Ferme l'interface de communication et vide les buffers d'envoi et de réception
//Si 'mode'==1, la communication est coupée sans regarder s'il reste des données a transmettre
//Si 'mode'!=1, la fonction ne ferme pas l'interface de communication s'il reste des données a transmettre
//et retourne 5
//Retourne 0 en cas de succes (communication terminée) et 5 s'il reste des données a transmettre
int Serial_Close(int mode);

//Copie l'octet numéro 'index' du buffer de réception vers 'dest' si 'index' ne dépasse pas les données du buffer
//Retourne 0 en cas de succes, 1 si 'index' dépasse les données du buffer, 3 si la communication n'est pas établie
int Serial_Peek(int index, unsigned char *dest);

//Récupère le statut de la connexion
//Retourne 1 si la connexion est établie, 3 sinon
int Serial_IsOpen(void);


#endif

#ifndef _SYSCALL
#define _SYSCALL

/**********************************************************/
/** Les syscall suivants servent � l'acces au port s�rie **/
/** Le buffer de r�ception fait 1ko,                     **/
/** et le buffer d'envoi fait 256 octets.                **/
/**********************************************************/

//Lit un caract�re du buffer de r�ception et le copie a l'adresse point�e par 'dest'
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas �tablie
int Serial_ReadByte(unsigned char *dest);

//Lit 'max' octets du buffer de r�ception et les �crit dans 'dest'
//'size' donne le nombre d'octets lu
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas �tablie
int Serial_ReadBytes(unsigned char *dest, int max, short *size);

//Ecrit 'byte' dans le buffer d'envoi
//Retourne 0
int Serial_WriteByte(unsigned char byte);

//Ecrit 'size' octets dans le buffer d'envoi depuis l'adresse 'scr'
//Retourne 0 en cas de succes, 2 si le buffer est trop plein, 3 si la connexion n'est pas �tablie
int Serial_WriteBytes(unsigned char *src, int size);

//FIFO = first in first out
int Serial_WriteByteFIFO(unsigned char byte);

//Retourne la taille des donn�es du buffer de r�ception
int Serial_GetRxBufferSize(void);

//Retourne l'espace disponible dans le buffer d'envoi
int Serial_GetTxBufferFreeCapacity(void);

//Vide le buffer de r�ception
//Retourne 0 en cas de succes, 3 si la connexion n'est pas �tablie
int Serial_ClearReceiveBuffer(void);

//Vide le buffer d'envoi
//Retourne 0
int Serial_ClearTransmitBuffer(void);

//Ouvre et pr�pare l'interface de communication
//Pour plus de d�tails, consulter fxreverse-doc-1.pdf ci joint
int Serial_Open(unsigned char *conf);

//Ferme l'interface de communication et vide les buffers d'envoi et de r�ception
//Si 'mode'==1, la communication est coup�e sans regarder s'il reste des donn�es a transmettre
//Si 'mode'!=1, la fonction ne ferme pas l'interface de communication s'il reste des donn�es a transmettre
//et retourne 5
//Retourne 0 en cas de succes (communication termin�e) et 5 s'il reste des donn�es a transmettre
int Serial_Close(int mode);

//Copie l'octet num�ro 'index' du buffer de r�ception vers 'dest' si 'index' ne d�passe pas les donn�es du buffer
//Retourne 0 en cas de succes, 1 si 'index' d�passe les donn�es du buffer, 3 si la communication n'est pas �tablie
int Serial_Peek(int index, unsigned char *dest);

//R�cup�re le statut de la connexion
//Retourne 1 si la connexion est �tablie, 3 sinon
int Serial_IsOpen(void);


#endif

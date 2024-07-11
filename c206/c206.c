/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	
	//set everything to NULL
	list->firstElement = NULL;
	list->activeElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

	//while there are elements in the list, delete them
	while (list->firstElement != NULL) {
		DLLElementPtr temp = list->firstElement;
		list->firstElement = list->firstElement->nextElement;
		if (temp != NULL)
			free(temp);
	}

	//set everything to NULL
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->activeElement = NULL;
	
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

	//create new first element
	DLLElementPtr new_first_element = malloc(sizeof(struct DLLElement));

	//if there is not enough memory for the new first element, call DLL_Error()
	if (new_first_element == NULL) {
		DLL_Error();
		return;
	}

	//create new first element, where the next element is the current first element
	//and the previous element is NULL
	new_first_element->data = data;
	new_first_element->nextElement = list->firstElement;
	new_first_element->previousElement = NULL;

	//if there is a node after the current first element,
	//set its previous element (current first) to the new first element
	if (list->firstElement != NULL) {
		list->firstElement->previousElement = new_first_element;
	}


	//if there is no node after the current first element,
	//set the first element to the new first element and
	//set the last element to the new first element
	if (list->firstElement == NULL) {
		list->lastElement = new_first_element;
	}

	//set the first element to the new first element
	list->firstElement = new_first_element;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

	//create new last element
	DLLElementPtr new_last_element = malloc(sizeof(struct DLLElement));

	//if there is not enough memory for the new last element, call DLL_Error()
	if (new_last_element == NULL) {
		DLL_Error();
		return;
	}

	//create new last element, where the previous element is the current last element
	//and the next element is NULL
	new_last_element->data = data;
	new_last_element->previousElement = list->lastElement;
	new_last_element->nextElement = NULL;

	//if there is a node before the current last element,
	//set its next element (current last) to the new last element
	if (list->lastElement != NULL) {
		list->lastElement->nextElement = new_last_element;
	}
	
	//if there is no node before the current last element,
	//set the first element to the new last element and
	//set the last element to the new last element
	if (list->lastElement == NULL) {
		list->firstElement = new_last_element;
		list->lastElement = list->firstElement;
	}

	//set the last element to the new last element
	list->lastElement = new_last_element;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

	//set the first element as the active element
	list->activeElement = list->firstElement;

}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

	//set the last element as the active element
	list->activeElement = list->lastElement;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

	//if the first element is NULL, call DLL_Error()
	if (list->firstElement == NULL) {
		DLL_Error();
		return;
	}

	//set the data of the first element to the dataPtr
	*dataPtr = list->firstElement->data;

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

	//if the last element is NULL, call DLL_Error()
	if (list->lastElement == NULL) {
		DLL_Error();
		return;
	}

	//set the data of the last element to the dataPtr
	*dataPtr = list->lastElement->data;

}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

	//if the first element is NULL, return
	if (list->firstElement == NULL) {
		return;
	}

	//if the active element is the first element, set it to NULL
	if (list->activeElement == list->firstElement) {
		list->activeElement = NULL;
	}

	//create a temporary pointer to the first element
	DLLElementPtr temp = list->firstElement;
  
	//set the first element to the next element
	list->firstElement = list->firstElement->nextElement;

	//free the temporary pointer
	free(temp);
	temp = NULL;

	//if the first element is not NULL, set its previous element to NULL
	if (list->firstElement != NULL) {
		list->firstElement->previousElement = NULL;
	}

	//if the first element is NULL, set the last element to NULL (the list is empty)
	if (list->firstElement == NULL) {
		list->lastElement = NULL;
	}

}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

	//if the last element is NULL, return
	if (list->lastElement == NULL) {
		return;
	}

	//if the active element is the last element, set it to NULL
	if (list->activeElement == list->lastElement) {
		list->activeElement = NULL;
	}

	//create a temporary pointer to the penultimate element
	DLLElementPtr penultimate_element = 
	list->lastElement->previousElement;

	//if the penultimate element is not NULL, set its next element to NULL
	if (penultimate_element != NULL)
		penultimate_element->nextElement = NULL;

	//free the last element
	DLLElementPtr temp = list->lastElement;
	free (temp);
	temp = NULL;

	//set the last element to the penultimate element
	list->lastElement = penultimate_element;
	
	//if the last element is NULL, set the first element to NULL (the list is empty)
	if (list->lastElement == NULL) {
		list->firstElement = NULL;
	}

}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

	//if the active element is NULL or the last element, return
	if (list->activeElement == NULL || 
			list->activeElement == list->lastElement) {
		return;
	}

	//create a temporary pointer to the element after and after the active element
	DLLElementPtr temp = list->activeElement->nextElement->nextElement;

	//create a temporary pointer to the element after the active element
	DLLElementPtr temp2 = list->activeElement->nextElement;
	
	//set the next element of the active element to the element 
	//after the element after the active element
	list->activeElement->nextElement = temp;

	//if the element after the element after the active element is not NULL,
	//set its previous element to the active element
	if (temp != NULL){
		temp->previousElement = list->activeElement;
	}

	//if the element after the element after the active element is NULL,
	//set the last element to the active element
	if (temp == NULL) {
		list->lastElement = list->activeElement;
	}

	//free the element after the active element
	free(temp2);
	temp2 = NULL;
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

	//if the active element is NULL or the first element, return
	if (list->activeElement == NULL || 
			list->activeElement == list->firstElement) {
		return;
	}

	//create a temporary pointer to the element before and before the active element
	DLLElementPtr temp = list->activeElement->previousElement->previousElement;

	//create a temporary pointer to the element before the active element
	DLLElementPtr temp2 = list->activeElement->previousElement;

	//if the element before the element before the active element is not NULL,
	//set its next element to the active element
	if (temp != NULL){
		temp->nextElement = list->activeElement;
		list->activeElement->previousElement = temp;
	}

	//if the element before the element before the active element is NULL,
	//set the first element to the active element
	if (temp == NULL) {
		list->firstElement = list->activeElement;
	}

	//free the element before the active element
	free(temp2);
	temp2 = NULL;

}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

	//if the active element is NULL, return
	if (list->activeElement == NULL) {
		return;
	}

	//create a new element
	DLLElementPtr new_element = malloc(sizeof(struct DLLElement));

	//if there is not enough memory for the new element, call DLL_Error()
	if (new_element == NULL) {
		DLL_Error();
		return;
	}

	//create a temporary pointer to the element after the active element
	DLLElementPtr active = list->activeElement;

	//set the data of the new element to the data
	new_element->data = data;

	//if there is an element after the active element,
	//and there is an element before the active element,
	//set the previous element of the new element to the active element
	//and the next element of the new element to the element after the active element
	//and do the same for the element after the active element
	if (active->previousElement != NULL && active->nextElement != NULL) {
		new_element->previousElement = active;
		new_element->nextElement = active->nextElement;

	 	active->nextElement->previousElement = new_element;
	 	active->nextElement = new_element;
	}

	//if there is no element after the active element,
	//set the previous element of the new element to the active element
	//and the next element of the new element to NULL
	//and do the same for the active element
	if (active->nextElement == NULL) {
		new_element->previousElement = active;
		new_element->nextElement = NULL;

	 	active->nextElement = new_element;
		list->lastElement = new_element;
	}

}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

	//if the active element is NULL, return
	if (list->activeElement == NULL) {
		return;
	}

	//create a new element
	DLLElementPtr new_element = malloc(sizeof(struct DLLElement));

	//if there is not enough memory for the new element, call DLL_Error()
	if (new_element == NULL) {
		DLL_Error();
		return;
	}

	//create a temporary pointer to the element before the active element
	DLLElementPtr active = list->activeElement;

	//set the data of the new element to the data
	new_element->data = data;

	//if there is an element before the active element,
	//and there is an element after the active element,
	//set the previous element of the new element to the element before the active element
	//and the next element of the new element to the active element
	//and do the same for the element before the active element
	if (active->previousElement != NULL && active->nextElement != NULL) {
		new_element->previousElement = active->previousElement;
		new_element->nextElement = active;

	 	active->previousElement->nextElement = new_element;
	 	active->previousElement = new_element;
	}

	//if there is no element before the active element,
	//set the previous element of the new element to NULL
	//and the next element of the new element to the active element
	//and do the same for the active element
	if (active->previousElement == NULL) {
		new_element->previousElement = NULL;
		new_element->nextElement = active;

	 	active->previousElement = new_element;
		list->firstElement = new_element;
	}

}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

	//if the active element is NULL, call DLL_Error()
	if (list->activeElement == NULL) {
		DLL_Error();
		return;
	}

	//set the data of the active element to the dataPtr
	*dataPtr = list->activeElement->data;

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

	//if the active element is NULL, return
	if (list->activeElement == NULL) {
		return;
	}

	//set the data of the active element to the data
	list->activeElement->data = data;

}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

	//if the active element is NULL, return
	if (list->activeElement == NULL) {
		return;
	}

	//if there is an element after the active element,
	//set the active element to the element after the active element
	if (list->activeElement->nextElement != NULL)
		list->activeElement = list->activeElement->nextElement;
	//if there is no element after the active element,
	else
		list->activeElement = NULL;

}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

	//if the active element is NULL, return
	if (list->activeElement == NULL) {
		return;
	}
	
	//if there is an element before the active element,
	//set the active element to the element before the active element
	if (list->activeElement->previousElement != NULL) {
		list->activeElement = list->activeElement->previousElement;
	} else {
		//If the previous element is NULL, the list becomes inactive
		list->activeElement = NULL;
	}

}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	//if the active element is not NULL, return true
	return list->activeElement != NULL;
}

/* Konec c206.c */

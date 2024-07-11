/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

	if (Stack_IsEmpty(stack))
		return;

	char top = 0;

	//empty stack until left par or until stack is empty (which is error)
	while (!Stack_IsEmpty(stack)){

		//get top of stack
		Stack_Top(stack, &top);

		//if top is left par, remove it and break
		if (top == '('){
			Stack_Pop(stack);
			break;
		}

		//else add top to postfix expression and remove it from stack
		postfixExpression[*postfixExpressionLength] = top;
		(*postfixExpressionLength)++;
		Stack_Pop(stack);
	}

}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

	char top = 0;

	//get top of stack
	if(!Stack_IsEmpty(stack))
		Stack_Top(stack, &top);

	//if the current char is left par, push it to stack and return
	if (c == '('){
		Stack_Push(stack, c);
		return;
	}

	//if the current char is right par, empty stack until left par
	if (c == ')'){
		untilLeftPar(stack, postfixExpression, postfixExpressionLength);
		return;
	}

	//if the current char is operator (+,-), empty stack until operator 
	//with lower priority or until stack is empty
	if (c == '+' || c == '-'){

			//while stack is not empty and top of stack is operator with higher priority
			while (!Stack_IsEmpty(stack) && (top == '*' || top == '/' || 
																		   top == '+' || top == '-')){

				//add top of stack to postfix expression and remove it from stack
				postfixExpression[*postfixExpressionLength] = top;
				(*postfixExpressionLength)++;
				Stack_Pop(stack);

				//get new top of stack
				if (!Stack_IsEmpty(stack))
					Stack_Top(stack, &top);
			}

			//push current char to stack
			Stack_Push(stack, c);

		return;
	}

	//if the current char is operator (*,/), empty stack until operator
	//with lower priority or until stack is empty
	if (c == '*' || c == '/'){

		//while stack is not empty and top of stack is operator with higher priority
		while (!Stack_IsEmpty(stack) && (top == '*' || top == '/')){

			//add top of stack to postfix expression and remove it from stack
			postfixExpression[*postfixExpressionLength] = top;
			(*postfixExpressionLength)++;
			Stack_Pop(stack);

			//get new top of stack
			if (!Stack_IsEmpty(stack))
				Stack_Top(stack, &top);
		}

		//push current char to stack
		Stack_Push(stack, c);

		return;
	}


}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

	//alloc postfix expression and check if it was allocated
	char *postfixExpression = (char *) malloc(MAX_LEN * sizeof(char));
	unsigned int PostfixExpressionLength = 0;

	if (postfixExpression == NULL)
		return NULL;

	//init stack
	Stack stack;

	Stack_Init(&stack);

	//go through infix expression
	char symbol;
	const char* ptr = infixExpression;

	//while not end of string
	while ((*ptr) != '\0'){
		symbol = *(ptr++);

		//if end of string, add '=' to postfix expression, empty stack and break
		if (symbol == '='){
			untilLeftPar(&stack, postfixExpression, &PostfixExpressionLength);
			postfixExpression[PostfixExpressionLength] = '=';
			break;
		}

		//if it is opator, do operation
		if (symbol == '(' ||
				symbol == ')' || 
				symbol == '+' || 
				symbol == '-' || 
				symbol == '*' || 
				symbol == '/'){
			doOperation(&stack, symbol, postfixExpression, &PostfixExpressionLength);
		} 
		
		//if it is a variable, add it to postfix expression
		else {
			postfixExpression[PostfixExpressionLength] = symbol;
			PostfixExpressionLength++;
		}
	}

	//add null terminator to postfix expression
	postfixExpression[PostfixExpressionLength+1] = '\0';

	//dispose stack
	Stack_Dispose(&stack);

	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

	//convert value to char*
	char *ptr = (char*)&value;

	//push values to stack
	for (int i = 0; i < 4; i++) {
		char byte = *(ptr + i);
		Stack_Push(stack, byte);
	}

}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

	//if stack is empty, return
	if (Stack_IsEmpty(stack))
		return;

	//convert value to char*
	char *byte = (char*)value;
	char b1, b2, b3, b4;

	//pop values to variables
	Stack_Top(stack, &b4);
	Stack_Pop(stack);
	Stack_Top(stack, &b3);
	Stack_Pop(stack);
	Stack_Top(stack, &b2);
	Stack_Pop(stack);
	Stack_Top(stack, &b1);
	Stack_Pop(stack);

	//set variables to int
	*byte = b1;
	*(byte + 1) = b2;
	*(byte + 2) = b3;
	*(byte + 3) = b4;

}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

	//init result
	int result = 0;

	//convert infix expression to postfix expression
	char *postfixExpression = infix2postfix(infixExpression);

	//init stack
	Stack stack;

	Stack_Init(&stack);

	//go through postfix expression
	char symbol = 0;
	char* ptr = postfixExpression;

	//while not end of string
	while ((*ptr) != '\0'){
		symbol = *(ptr++);

		//if the symbol is '=', pop result from stack and break 
		if (symbol == '='){
			expr_value_pop(&stack, &result);
			break;
		}

		//if the symbol is variable, push its value to stack
		if ((symbol >= 'A' &&  symbol <= 'Z') ||
		    (symbol >= 'a' &&  symbol <= 'z')){

			//find variable in variableValues
			bool found = false;
			for (int i = 0; i < variableValueCount; i++){

				//if found, push its value to stack and break
				if (variableValues[i].name == symbol){
					expr_value_push(&stack, variableValues[i].value);
					found = true;
					break;
				}

			}

			//if not found, return false
			if (found)
				continue;
			else
				return false;
		}

		//if the symbol is operator, pop two values from stack, do operation and push result to stack
		int operation = 0;
		//1 - plus, 2 - minus, 3 - multiply, 4 - divide
		if (symbol == '+')
			operation = 1;
		else if (symbol == '-')
			operation = 2;
		else if (symbol == '*')
			operation = 3;
		else if (symbol == '/')
			operation = 4;

		//pop two values from stack, in reverse order
		int a, b;
		expr_value_pop(&stack, &b);
		expr_value_pop(&stack, &a);

		//do operation
		if (operation == 1)
			result = a + b;
		else if (operation == 2)
			result = a - b;
		else if (operation == 3)
			result = a * b;
		else if (operation == 4)
			result = a / b;

		//push result to stack
		expr_value_push(&stack, result);
	}

	//dispose stack
	Stack_Dispose(&stack);

	free (postfixExpression);

	*value = result;
	return true;
}

/* Konec c204.c */

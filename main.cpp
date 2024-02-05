#include <iostream>		//std::ostream
#include <stdexcept>	//std::runtime_error
#include <cstddef> 		//std::ptrdiff_t
#include <cassert>		//assert()
#include "oMultiset.h"
#include "elementNotFound.h"



//======================================== Sezione test int ========================================


typedef int typeInt;									///< typedef per int

/**
 * @brief Funtore di ordinamento crescente tra int.
 *
 * Stabilisce se tra due int il primo è minore del secondo.
 */
struct int_cresc {
	bool operator()(typeInt a, typeInt b) const {
		return a<b;
	}
};

/**
 * @brief Funtore di ordinamento crescente tra int.
 *
 * Stabilisce se tra due int il primo è maggiore del secondo.
 */
struct int_decresc {
	bool operator()(typeInt a, typeInt b) const {
		return a>b;
	}
};

/**
 * @brief Funtore di uguaglianza.
 *
 * Stabilisce se due int si equivalgono.
 */
struct int_eql {
	bool operator()(typeInt a, typeInt b) const {
		return a==b;
	}
};

typedef oMultiset <typeInt, int_cresc, int_eql> ordMsetInt;
typedef oMultiset <typeInt, int_cresc, int_eql>::const_iterator ordMsetIntConst_iterator;


/**
 * @brief Test su un multiset di int
 * 
 * @param ms1 multiset di int non const usato come partenza per fare i test
 */
void testMultisetInt(ordMsetInt &ms1) {
	std::cout << "********** Test oMultiset di interi **********" << std::endl;

	typeInt elemento0 = 0;									//Valore da inserire in testa
	typeInt elemento1 = 1;									//Valore da inserire 2 volte
	typeInt elemento2 = 2;									//Valore non presente per l'eccezione
	typeInt elemento3 = 3;									//Valore da inserire al centro
	typeInt elemento4 = 4;									//Valore da inserire al centropiù avanti, da elementsCount()
	typeInt elemento5 = 5;									//Valore da inserire alla fine
	typeInt elemento6 = 6;									//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeInt elemento7 = 7;									//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeInt elemento8 = 8;									//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeInt arrayDatiMset[5] = {5,4,3,2,1}; 				//Verrà usato più avanti nei test degli iteratori

	//TUTTE le stampe sono eseguite con l'operatore ridefinito << quindi fanno anche da test per esso.
	std::cout << "Stampa di oMultiset passato come argomento (costruttore di default): "  << std::endl << ms1 << std::endl;
	ordMsetInt ms3;											//costruttore di default
	std::cout << "Stampa di oMultiset non inizializzato manualmente (costruttore di default): "  << std::endl << ms3 << std::endl;
	ordMsetInt ms4(ms3);
	std::cout << "oMultiset creato con copyconstructor da oMultiset vuoto: "  << std::endl << ms4 << std::endl;
	ms1.add(elemento1);										//add con oMultiset vuoto
	ms1.add(elemento1);										//add di elemento già presente
	ms1.add(elemento0);										//add di elemento all'inizio
	ms1.add(elemento5);										//add di elemento alla fine
	ms1.add(elemento3);										//add di elemento nel centro
	std::cout << "Metodo add testato in ogni possibile caso, risultato: " << std::endl << "ms1: " << ms1 << std::endl;

	try {													//tentativo di rimozione di elemento non presente
		ms1.remove(elemento2);								//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {							//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento2 << std::endl;
	}

	ordMsetInt ms2(ms1); 									//copy constructor
	std::cout << "Creazione di oMultiset con copy constructor: " << std::endl << "ms2: " << ms2 << std::endl;
	ms2.clear();											//metodo clear per svuotare l'oMultiset (sfrutta il metodo remove)
	std::cout << "Metodo clear su ms2 (e verifica di non condivisione dei dati dopo copy constructor): " << std::endl << 
		"ms2: " << ms2 << std::endl << "ms1: " << ms1 << std::endl;
	try {													//tentativo di rimozione con oMultiset vuoto
		ms2.remove(elemento1);								//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {							//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento1 << std::endl;
	}
	std::cout << "Rimozione manuale di ogni elemento da ms1: " << std::endl;
	std::cout << "ms1: " << ms1 << std::endl;
	ms1.remove(elemento0);									//rimozione primo elemento
	std::cout << "ms1: " << ms1 << std::endl;
	ms1.remove(elemento5);									//rimozione ultimo elemento
	ms1.remove(elemento3);									//rimozione elemento al centro

	//metodo occursCount()
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 2);
	ms1.remove(elemento1);									//rimozione di una occorrenza
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 1);
	ms1.remove(elemento1);									//rimozione dell'ultimo elemento
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 0);
	
	//metodo elementsCount() su oMultiset vuoto
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 0);
	ms1.add(elemento0);
	ms1.add(elemento1);
	ms1.add(elemento1);
	ms1.add(elemento3);
	ms1.add(elemento4);
	ms1.add(elemento5);
	std::cout << "ms1: " << ms1 << std::endl;
	//metodo elementsCount() su oMultiset pieno (con un elemento con occorrenza > 1)
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 6);
	std::cout << "ms2: " << ms2 << std::endl;				//in questo momento ms2 è vuoto
	std::cout << "Per riempire di nuovo ms2 uso l'operatore di assegamento per fare ms2 = ms1." << std::endl;
	ms2 = ms1;												//opeartore =
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl ;
	std::cout << "Verifico l'uguaglianza tra i due set con l'operatore ==" << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(ms1 == ms2);
	assert(ms2 == ms1);

	std::cout << "test su opeartore == con una occorrenza di differenza:" << std::endl;
	ms1.remove(elemento1);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "test operatore == con un elemento in meno:" << std::endl;
	ms1.add(elemento1);
	ms1.remove(elemento3);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "Posso usare l'operatore = su due oMultiset non vuoti e cambiarne la dimensione: ms2 = ms1" << std::endl;
	ms2 = ms1;												//assegnamento di un oMultiset vuoto ad un oMultiset pieno
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == ms2.elementsCount());
	assert(ms1 == ms2);
	std::cout << "Svuoto un oMultiset con clear()" << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Posso anche usare la clear() su oMultiset appena svuotato, non succede nulla: " << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Cambo gli elementi contenuti in ms1: " << std::endl;
	ms1.add(elemento8);
	ms1.add(elemento7);
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "L'assegnamento funziona anche quando il contenuto è completamente diverso: ms2 = ms1" << std::endl;
	ms2 = ms1;
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "Test metodo contains:" << std::endl;
	std::cout << "ms1.contains(" << elemento7 << ")? " << ms1.contains(elemento7) << 
				", ms1.contains(" << elemento8 << ")? " << ms1.contains(elemento8) << 
				", ms1.contains(" << elemento1 << ")? " << ms1.contains(elemento1) << std::endl;
	assert(ms1.contains(elemento7));
	assert(ms1.contains(elemento8));
	assert(!(ms1.contains(elemento1)));

	std::cout << "Test costruttore tramite iteratori num.1, creazione di due oMultiset con funtore Comp diverso:" << std::endl;
	ordMsetInt ms5(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms5: " << ms5 << std::endl;
	oMultiset<typeInt, int_decresc, int_eql> ms8(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms8: " << ms8 << std::endl;
	std::cout << "ms5 == ms8? " << (ms5 == ms8) << ". E ms8 == ms5? " << (ms8 == ms5) << "." << std::endl;
	assert(ms5 == ms8);
	assert(ms8 == ms5);

	std::cout << "Test costruttore tramite iteratori num.2:" << std::endl;
	ordMsetInt ms6(ms5.begin(), ms5.end());
	std::cout << "ms6: " << ms6 << std::endl;

	std::cout << "Test costruttore tramite iteratori num.3(da oMultiset vuoto):" << std::endl;
	ms5.clear();
	ordMsetInt ms7(ms5.begin(), ms5.end());
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;
	std::cout << "Verifico l'assenza di condivisione di dati aggiungendo un elemento da ms7: " << std::endl;
	ms7.add(elemento6);
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;

	std::cout << "Test iteratori:" << std::endl;
	ordMsetIntConst_iterator b,e;
	b = ms5.begin();
	e = ms5.end();
	std::cout << "Verifico che ms5.begin() == ms5.end(): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	e = ms7.end();
	std::cout << "Verifico che ms7.begin() != ms7.end(): " << (b != e) << std::endl;
	assert(b != e);
	std::cout << "Verifico che *b == ms7.begin(): " << (*b == *(ms7.begin())) << std::endl;
	assert(*b == *(ms7.begin()));
	std::cout << "Verifico che (++ms7.begin()) == ms7.end(): " << (++b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	std::cout << "Verifico che (ms7.begin()++) != ms7.end(): " << (b++ == e) << std::endl;
	assert(b == e);
	std::cout << "Verifico che ora il postincremento di b abbia funzionato, (b == e): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	ordMsetIntConst_iterator e2(b);							//copyconstructor dell'iteratore, ora b == e2 == ms7.begin()
	std::cout << "Verifico copy construcotr iteratore (b == e2): " << (b == e2) << std::endl;
	assert(b == e2);
	std::cout << "Se incremento e2 otterrò un iteratore che punta allo stesso indirizzo di ms7.end() == e: " << (++e2 == e) << std::endl;
	assert(e2 == e);

	ms1.clear();											//pulisco ms1 per passarlo al test della const
	std::cout << std::endl << std::endl << std::endl;
}



//======================================== Sezione test const int ========================================


/**
 * @brief Test su un multiset costante di int 
 * 
 * @param ms1 multiset di int const usato come partenza per fare i test
 */
void testConstMultisetInt(const ordMsetInt &ms1) {
	std::cout << "********** Test oMultiset const di interi **********" << std::endl;

	typeInt arrayDatiMset[6] = {5,4,3,2,1,1}; 				//Verrà usato più avanti nei test degli iteratori

	//TUTTE le stampe sono eseguite con l'operatore ridefinito << quindi fanno anche da test per esso.
	std::cout << "Stampa di oMultiset passato come argomento (costruttore di default): "  << std::endl << ms1 << std::endl;
	//non posso eseguire una add su oMultiset const
	//ms1.add(elemento1);
	//non posso eseguire nessuna remove su oMultiset const
	//non posso ricevere l'eccezione di tentata rimozione perché non posso usare la remove
	//ms1.remove(elemento1);
	//non posso usare il metodo clear su oMultiset const
	//ms1.clear();
	std::cout << "Test costruttore tramite iteratori num.1, creazione di due oMultiset con funtore Comp diverso:" << std::endl;
	const ordMsetInt ms2(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms2: " << ms2 << std::endl;
	const oMultiset<typeInt, int_decresc, int_eql> ms3(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms3: " << ms3 << std::endl;
	std::cout << "ms2 == ms3? " << (ms2 == ms3) << ". E ms3 == ms2? " << (ms3 == ms2) << "." << std::endl;
	assert(ms2 == ms3);
	assert(ms3 == ms2);
	std::cout << "Test costruttore tramite iteratori num.2:" << std::endl;
	const ordMsetInt ms4(ms2.begin(), ms2.end());
	std::cout << "ms4: " << ms4 << std::endl;
	std::cout << "Test costruttore tramite iteratori num.3(da oMultiset vuoto):" << std::endl;
	//ms2.clear();
	const ordMsetInt ms5(ms1.begin(), ms1.end());
	std::cout << "ms1: " << ms1 << std::endl << "ms5: " << ms5 << std::endl;
	std::cout << "Test iteratori:" << std::endl;
	ordMsetIntConst_iterator b,e;
	b = ms5.begin();
	e = ms5.end();
	std::cout << "Verifico che ms5.begin() == ms5.end(): " << (b == e) << std::endl;
	assert(b == e);
	std::cout << "ms2: " << ms2 << std::endl;
	b = ms2.begin();
	e = ms2.end();
	std::cout << "Verifico che ms2.begin() != ms2.end(): " << (b != e) << std::endl;
	assert(b != e);
	std::cout << "Verifico che *b == ms2.begin(): " << (*b == *(ms2.begin())) << std::endl;
	assert(*b == *(ms2.begin()));
	
	const ordMsetInt ms6(arrayDatiMset, arrayDatiMset+1);
	std::cout << "ms6: " << ms6 << std::endl;
	b = ms6.begin();
	e = ms6.end();
	std::cout << "Verifico che (++ms6.begin()) == ms6.end(): " << (++b == e) << std::endl;
	assert(b == e);											//b è stato incrementato sopra non devo farlo di nuovo
	b = ms6.begin();
	std::cout << "Verifico che (ms6.begin()++) != ms6.end(): " << (b++ != e) << std::endl;
	std::cout << "Verifico che ora il postincremento di b abbia funzionato, (b == e): " << (b == e) << std::endl;
	assert(b == e);
	b = ms6.begin();
	ordMsetIntConst_iterator e2(b);							//copyconstructor dell'iteratore, ora b == e2 == ms5.begin()
	std::cout << "Verifico copy construcotr iteratore (b == e2): " << (b == e2) << std::endl;
	assert(b == e2);
	std::cout << "Se incremento e2 otterrò un iteratore che punta allo stesso indirizzo di ms6.end() == e: " << (++e2 == e) << std::endl;
	assert(e2 == e);

	const ordMsetInt ms7(arrayDatiMset+5, arrayDatiMset+6);
	const ordMsetInt ms8(arrayDatiMset+4, arrayDatiMset+6);
	const ordMsetInt ms9(arrayDatiMset, arrayDatiMset+6);
	std::cout << "ms1: " << ms1 << std::endl;
	std::cout << "ms5: " << ms5 << std::endl;
	std::cout << "ms7: " << ms7 << std::endl;
	std::cout << "ms8: " << ms8 << std::endl;
	std::cout << "ms9: " << ms9 << std::endl;
	//metodo elementsCount() su oMultiset vuoto
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 0);
	//metodo elementsCount() su oMultiset pieno (con un elemento con occorrenza > 1)
	std::cout << "Elementi presenti in ms9: " << ms9.elementsCount() << std::endl;
	assert(ms9.elementsCount() == 6);
	std::cout << "Due oMultiset vuoti sono considerati uguali." << std::endl;
	std::cout << "ms1 == ms5? " << (ms1 == ms5) << ". E ms5 == ms1? " << (ms5 == ms1) << "." << std::endl;
	assert(ms1 == ms5);
	assert(ms5 == ms1);
	//non posso utilizzare l'operatore di assegnamento a causa della const
	//ms2 = ms1;												//opeartore =
	std::cout << "test su opeartore == con una occorrenza di differenza:" << std::endl;
	std::cout << "ms7 == ms8? " << (ms7 == ms8) << ". E ms8 == ms7? " << (ms8 == ms7) << "." << std::endl;
	assert(!(ms7 == ms8));
	assert(!(ms8 == ms7));
	std::cout << "test operatore == con un elemento in meno:" << std::endl;
	std::cout << "ms1 == ms7? " << (ms1 == ms7) << ". E ms7 == ms1? " << (ms7 == ms1) << "." << std::endl;
	assert(!(ms1 == ms7));
	assert(!(ms7 == ms1));
	std::cout << "Test metodo contains:" << std::endl;
	std::cout << "ms9.contains(" << 1 << ")? " << ms9.contains(1) << 
				", ms9.contains(" << 2 << ")? " << ms9.contains(2) << 
				", ms9.contains(" << 9 << ")? " << ms9.contains(9) << std::endl;
	std::cout << std::endl << std::endl << std::endl;
	assert(ms9.contains(1));
	assert(ms9.contains(2));
	assert(!(ms9.contains(9)));
	
}



//======================================== Sezione test string ========================================


typedef std::string typeString;								///< typedef per std::string

/**
 * @brief Funtore di ordinamento crescente tra string
 *
 * Stabilisce se tra due oggetti di tipo string il primo è minore del secondo.
 */
struct string_cresc {
	bool operator()(const typeString &a, const typeString &b) const {
		return a.length() < b.length();
	}
};

/**
 * @brief Funtore di ordinamento decrescente tra string
 *
 * Stabilisce se tra due oggetti di tipo string il primo è maggiore del secondo.
 */
struct string_decresc {
	bool operator()(const typeString &a, const typeString &b) const {
		return a.length() > b.length();
	}
};

/**
 * @brief Funtore di uguaglianza.
 *
 * Stabilisce se due oggetti di tipo string si equivalgono.
 */
struct string_eql {
	bool operator()(const typeString &a, const typeString &b) const {
		return a == b;
	}
};

typedef oMultiset <typeString, string_cresc, string_eql> ordMsetString;											///< typedef per multiset crescente di string
typedef oMultiset <typeString, string_cresc, string_eql>::const_iterator ordMsetStringConst_iterator;			///< typedef per multiset crescente di string

void testMultisetString(ordMsetString &ms1) {
	std::cout << "********** Test oMultiset di stringhe **********" << std::endl;

	typeString elemento0 = "a";								//Valore da inserire in testa
	typeString elemento1 = "ab";							//Valore da inserire 2 volte
	typeString elemento2 = "abc";							//Valore non presente per l'eccezione
	typeString elemento3 = "abcd";							//Valore da inserire al centro
	typeString elemento4 = "abcde";							//Valore da inserire al centro più avanti, da elementsCount()
	typeString elemento5 = "abcdef";						//Valore da inserire alla fine
	typeString elemento6 = "abcdefg";						//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeString elemento7 = "abcdefgh";						//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeString elemento8 = "abcdefghi";						//Valore usato più avanti nei test dell'operatore d'assegnamento
	typeString arrayDatiMset[5] = {"abcde", "abcd", "abc", "ab", "a"}; 					//Verrà usato più avanti nei test degli iteratori

	//TUTTE le stampe sono eseguite con l'operatore ridefinito << quindi fanno anche da test per esso.
	std::cout << "Stampa di oMultiset passato come argomento (costruttore di default): "  << std::endl << ms1 << std::endl;
	ordMsetString ms3;											//costruttore di default
	std::cout << "Stampa di oMultiset non inizializzato manualmente (costruttore di default): "  << std::endl << ms3 << std::endl;
	ordMsetString ms4(ms3);
	std::cout << "oMultiset creato con copyconstructor da oMultiset vuoto: "  << std::endl << ms4 << std::endl;
	ms1.add(elemento1);										//add con oMultiset vuoto
	ms1.add(elemento1);										//add di elemento già presente
	ms1.add(elemento0);										//add di elemento all'inizio
	ms1.add(elemento5);										//add di elemento alla fine
	ms1.add(elemento3);										//add di elemento nel centro
	std::cout << "Metodo add testato in ogni possibile caso, risultato: " << std::endl << "ms1: " << ms1 << std::endl;

	try {													//tentativo di rimozione di elemento non presente
		ms1.remove(elemento2);								//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {							//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento2 << std::endl;
	}

	ordMsetString ms2(ms1); 									//copy constructor
	std::cout << "Creazione di oMultiset con copy constructor: " << std::endl << "ms2: " << ms2 << std::endl;
	ms2.clear();											//metodo clear per svuotare l'oMultiset (sfrutta il metodo remove)
	std::cout << "Metodo clear su ms2 (e verifica di non condivisione dei dati dopo copy constructor): " << std::endl << 
		"ms2: " << ms2 << std::endl << "ms1: " << ms1 << std::endl;
	try {													//tentativo di rimozione con oMultiset vuoto
		ms2.remove(elemento1);								//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {							//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento1 << std::endl;
	}
	std::cout << "Rimozione manuale di ogni elemento da ms1: " << std::endl;
	std::cout << "ms1: " << ms1 << std::endl;
	ms1.remove(elemento0);									//rimozione primo elemento
	std::cout << "ms1: " << ms1 << std::endl;
	ms1.remove(elemento5);									//rimozione ultimo elemento
	ms1.remove(elemento3);									//rimozione elemento al centro

	//metodo occursCount()
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 2);
	ms1.remove(elemento1);									//rimozione di una occorrenza
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 1);
	ms1.remove(elemento1);									//rimozione dell'ultimo elemento
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 0);
	
	//metodo elementsCount() su oMultiset vuoto
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 0);
	ms1.add(elemento0);
	ms1.add(elemento1);
	ms1.add(elemento1);
	ms1.add(elemento3);
	ms1.add(elemento4);
	ms1.add(elemento5);
	std::cout << "ms1: " << ms1 << std::endl;
	//metodo elementsCount() su oMultiset pieno (con un elemento con occorrenza > 1)
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 6);
	std::cout << "ms2: " << ms2 << std::endl;				//in questo momento ms2 è vuoto
	std::cout << "Per riempire di nuovo ms2 uso l'operatore di assegamento per fare ms2 = ms1." << std::endl;
	ms2 = ms1;												//opeartore =
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl ;
	std::cout << "Verifico l'uguaglianza tra i due set con l'operatore ==" << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(ms1 == ms2);
	assert(ms2 == ms1);

	std::cout << "test su opeartore == con una occorrenza di differenza:" << std::endl;
	ms1.remove(elemento1);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "test operatore == con un elemento in meno:" << std::endl;
	ms1.add(elemento1);
	ms1.remove(elemento3);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "Posso usare l'operatore = su due oMultiset non vuoti e cambiarne la dimensione: ms2 = ms1" << std::endl;
	ms2 = ms1;												//assegnamento di un oMultiset vuoto ad un oMultiset pieno
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == ms2.elementsCount());
	assert(ms1 == ms2);
	std::cout << "Svuoto un oMultiset con clear()" << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Posso anche usare la clear() su oMultiset appena svuotato, non succede nulla: " << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Cambo gli elementi contenuti in ms1: " << std::endl;
	ms1.add(elemento8);
	ms1.add(elemento7);
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "L'assegnamento funziona anche quando il contenuto è completamente diverso: ms2 = ms1" << std::endl;
	ms2 = ms1;
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "Test metodo contains:" << std::endl;
	std::cout << "ms1.contains(" << elemento7 << ")? " << ms1.contains(elemento7) << 
				", ms1.contains(" << elemento8 << ")? " << ms1.contains(elemento8) << 
				", ms1.contains(" << elemento1 << ")? " << ms1.contains(elemento1) << std::endl;
	assert(ms1.contains(elemento7));
	assert(ms1.contains(elemento8));
	assert(!(ms1.contains(elemento1)));

	std::cout << "Test costruttore tramite iteratori num.1, creazione di due oMultiset con funtore Comp diverso:" << std::endl;
	ordMsetString ms5(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms5: " << ms5 << std::endl;
	oMultiset<typeString, string_decresc, string_eql> ms8(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms8: " << ms8 << std::endl;
	std::cout << "ms5 == ms8? " << (ms5 == ms8) << ". E ms8 == ms5? " << (ms8 == ms5) << "." << std::endl;
	assert(ms5 == ms8);
	assert(ms8 == ms5);

	std::cout << "Test costruttore tramite iteratori num.2:" << std::endl;
	ordMsetString ms6(ms5.begin(), ms5.end());
	std::cout << "ms6: " << ms6 << std::endl;

	std::cout << "Test costruttore tramite iteratori num.3(da oMultiset vuoto):" << std::endl;
	ms5.clear();
	ordMsetString ms7(ms5.begin(), ms5.end());
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;
	std::cout << "Verifico l'assenza di condivisione di dati aggiungendo un elemento da ms7: " << std::endl;
	ms7.add(elemento6);
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;

	std::cout << "Test iteratori:" << std::endl;
	ordMsetStringConst_iterator b,e;
	b = ms5.begin();
	e = ms5.end();
	std::cout << "Verifico che ms5.begin() == ms5.end(): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	e = ms7.end();
	std::cout << "Verifico che ms7.begin() != ms7.end(): " << (b != e) << std::endl;
	assert(b != e);
	std::cout << "Verifico che *b == ms7.begin(): " << (*b == *(ms7.begin())) << std::endl;
	assert(*b == *(ms7.begin()));
	std::cout << "Verifico che (++ms7.begin()) == ms7.end(): " << (++b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	std::cout << "Verifico che (ms7.begin()++) != ms7.end(): " << (b++ == e) << std::endl;
	assert(b == e);
	std::cout << "Verifico che ora il postincremento di b abbia funzionato, (b == e): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	ordMsetStringConst_iterator e2(b);							//copyconstructor dell'iteratore, ora b == e2 == ms7.begin()
	std::cout << "Verifico copy construcotr iteratore (b == e2): " << (b == e2) << std::endl;
	assert(b == e2);
	std::cout << "Se incremento e2 otterrò un iteratore che punta allo stesso indirizzo di ms7.end() == e: " << (++e2 == e) << std::endl;
	assert(e2 == e);

	std::cout << std::endl << std::endl << std::endl;
}



//======================================== Sezione test 3D Point ========================================


/**
 * @brief Punto a tre dimensioni.
 * 
 * Struct che rappresenta un punto a tre dimensioni.
 */
struct Point3D {
	double _x;													///< Coordinata x di un Point3D
	double _y;													///< Coordinata y di un Point3D
	double _z;													///< Coordinata z di un Point3D
	
	/**
	 * @brief Costruttore di default. (METODO FONDAMENTALE)
	 * 
	 */
	Point3D () : _x(0), _y(0), _z(0) {}
	
	/**
	 * @brief Copy constructor. (METODO FONDAMENTALE)
	 * 
	 * @param other 
	 */
	Point3D (const Point3D &other) : _x(other._x), _y(other._y), _z(other._z) {}

	/**
	 * @brief Operatore di assegnamento. (METODO FONDAMENTALE)
	 * 
	 * @param other Oggetto Point3D contenete i valori da copiare nell'oggetto corrente.
	 * @return oggetto corrente dereferenziato
	 */
	Point3D& operator= (const Point3D &other) {
		if(this!=&other) {
			_x = other._x;									///< typedef per std::string
			_y = other._y;
			_z = other._z;  
		}
		return *this;
	}
	
	/**
	 * @brief Distruttore.
	 */
	~Point3D(){}

	/**
	 * @brief Costruttore secondario.
	 * 
	 * @param xValue Valore da assegnare ad _x rappresenta la coordinata X.
	 * @param yValue Valore da assegnare ad _y rappresenta la coordinata Y.
	 * @param zValue Valore da assegnare ad _z rappresenta la coordinata Z.
	 */
	Point3D (double xValue, double yValue, double zValue) : _x(xValue), _y(yValue), _z(zValue) {}
	
	/**
	 * @brief Ridefinizione dell'operatore== per la classe Point3D.
	 * 
	 * @param other oggetto Point3D su cui eseguire il confronto di uguaglianza.
	 * @return true se i due Point3D si equivalgono in tutte le coordinate.
	 * @return false se i due Point3D non si equivalgono in tutte le coordinate.
	 */
	bool operator== (const Point3D &other) const {
		return(_x == other._x && _y == other._y && _z == other._z);
	}

	/**
	 * @brief Ridefinizione dell'operatore di stream<< per la classe Point3D
	 * 
	 * @param os stream di output.
	 * @param point oggetto Point3D da aggiungere allo stream per la stampa.
	 * @return stream di output std::ostream.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Point3D &point) {
		std::cout << "(" << point._x << ", " << point._y << ", " << point._z << ")";
		return os;
	} 
};

typedef Point3D typePoint3D;

/**
 * @brief Funtore di ordinamento crescente tra Point3D
 *
 * Stabilisce se tra due oggetti di tipo Point3D il primo è minore del secondo.
 */
struct point_cresc {
	bool operator()(const typePoint3D &a, const typePoint3D &b) const {
		return a._x < b._x;
	}
};


/**
 * @brief Funtore di ordinamento decrescente tra Point3D
 *
 * Stabilisce se tra due oggetti di tipo Point3D il primo è maggiore del secondo.
 */
struct point_decresc {
	bool operator()(const typePoint3D &a, const typePoint3D &b) const {
		return a._x > b._x;
	}
};


/**
 * @brief Funtore di uguaglianza.
 *
 * Stabilisce se due oggetti di tipo Point3D si equivalgono.
 */
struct point_eql {
	bool operator()(const typePoint3D &a, const typePoint3D &b) const {
		return a == b;
	}
};


typedef oMultiset <typePoint3D, point_cresc, point_eql> ordMsetPoint3D;										///< typedef per multiset crescente di Point3D
typedef oMultiset <typePoint3D, point_cresc, point_eql>::const_iterator ordMsetPoint3DConst_iterator;		///< typedef per const_iterator di multiset crescente di Point3D


/**
 * @brief Test su un multiset di Point3D
 * 
 * @param ms1 multiset di Point3D non const usato come partenza per fare i test
 */
void testMultisetPoint3D(ordMsetPoint3D &ms1) {
	std::cout << "********** Test oMultiset di Point3D **********" << std::endl;

	typePoint3D elemento0 (0, 0, 0);								//Valore da inserire in testa
	typePoint3D elemento1 (1, 1, 1);								//Valore da inserire 2 volte
	typePoint3D elemento2 (2, 2, 2);								//Valore non presente per l'eccezione
	typePoint3D elemento3 (3, 3, 3);								//Valore da inserire al centro
	typePoint3D elemento4 (4, 4, 4);								//Valore da inserire al centropiù avanti, da elementsCount()
	typePoint3D elemento5 (5, 5, 5);								//Valore da inserire alla fine
	typePoint3D elemento6 (6, 6, 6);								//Valore usato più avanti nei test dell'operatore d'assegnamento
	typePoint3D elemento7 (7, 7, 7);								//Valore usato più avanti nei test dell'operatore d'assegnamento
	typePoint3D elemento8 (8, 8, 8);								//Valore usato più avanti nei test dell'operatore d'assegnamento
	typePoint3D arrayDatiMset[5] =
		{elemento5, elemento4, elemento3, elemento2, elemento1}; 	//Verrà usato più avanti nei test degli iteratori

		//TUTTE le stampe sono eseguite con l'operatore ridefinito << quindi fanno anche da test per esso.
	std::cout << "Stampa di oMultiset passato come argomento (costruttore di default): "  << std::endl << ms1 << std::endl;
	ordMsetPoint3D ms3;												//costruttore di default
	std::cout << "Stampa di oMultiset non inizializzato manualmente (costruttore di default): "  << std::endl << ms3 << std::endl;
	ordMsetPoint3D ms4(ms3);
	std::cout << "oMultiset creato con copyconstructor da oMultiset vuoto: "  << std::endl << ms4 << std::endl;
	ms1.add(elemento1);												//add con oMultiset vuoto
	ms1.add(elemento1);												//add di elemento già presente
	ms1.add(elemento0);												//add di elemento all'inizio
	ms1.add(elemento5);												//add di elemento alla fine
	ms1.add(elemento3);												//add di elemento nel centro
	std::cout << "Metodo add testato in ogni possibile caso, risultato: " << std::endl << "ms1: " << ms1 << std::endl;

	try {															//tentativo di rimozione di elemento non presente
		ms1.remove(elemento2);										//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {									//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento2 << std::endl;
	}

	ordMsetPoint3D ms2(ms1); 										//copy constructor
	std::cout << "Creazione di oMultiset con copy constructor: " << std::endl << "ms2: " << ms2 << std::endl;
	ms2.clear();													//metodo clear per svuotare l'oMultiset (sfrutta il metodo remove)
	std::cout << "Metodo clear su ms2 (e verifica di non condivisione dei dati dopo copy constructor): " << std::endl << 
		"ms2: " << ms2 << std::endl << "ms1: " << ms1 << std::endl;
	try {															//tentativo di rimozione con oMultiset vuoto
		ms2.remove(elemento1);										//operazione vietata: lancerà un'eccezione custom
	} catch(elementNotFound &msg) {									//eccezione custom ricevuta e gestita
		std::cout << msg.what() << " Elemento: " << elemento1 << std::endl;
	}
	std::cout << "Rimozione manuale di ogni elemento da ms1: " << std::endl;
	std::cout << "ms1: " << ms1 << std::endl;
	ms1.remove(elemento0);											//rimozione primo elemento
	std::cout << "ms1: " << ms1 << std::endl;		
	ms1.remove(elemento5);											//rimozione ultimo elemento
	ms1.remove(elemento3);											//rimozione elemento al centro

	//metodo occursCount()
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 2);
	ms1.remove(elemento1);											//rimozione di una occorrenza
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 1);
	ms1.remove(elemento1);											//rimozione dell'ultimo elemento
	std::cout << "ms1: " << ms1 << " <== Occorrenze di " << elemento1 << ": "  << ms1.occurrsCount(elemento1) << std::endl;
	assert(ms1.occurrsCount(elemento1) == 0);
	
	//metodo elementsCount() su oMultiset vuoto
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 0);
	ms1.add(elemento0);
	ms1.add(elemento1);
	ms1.add(elemento1);
	ms1.add(elemento3);
	ms1.add(elemento4);
	ms1.add(elemento5);
	std::cout << "ms1: " << ms1 << std::endl;
	//metodo elementsCount() su oMultiset pieno (con un elemento con occorrenza > 1)
	std::cout << "Elementi presenti in ms1: " << ms1.elementsCount() << std::endl;
	assert(ms1.elementsCount() == 6);
	std::cout << "ms2: " << ms2 << std::endl;						//in questo momento ms2 è vuoto
	std::cout << "Per riempire di nuovo ms2 uso l'operatore di assegamento per fare ms2 = ms1." << std::endl;
	ms2 = ms1;														//opeartore =
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl ;
	std::cout << "Verifico l'uguaglianza tra i due set con l'operatore ==" << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(ms1 == ms2);
	assert(ms2 == ms1);

	std::cout << "test su opeartore == con una occorrenza di differenza:" << std::endl;
	ms1.remove(elemento1);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "test operatore == con un elemento in meno:" << std::endl;
	ms1.add(elemento1);
	ms1.remove(elemento3);
	std::cout << "ms1: " << ms1 << std::endl << "ms2: " << ms2 << std::endl;
	std::cout << "ms1 == ms2? " << (ms1 == ms2) << ". E ms2 == ms1? " << (ms2 == ms1) << "." << std::endl;
	assert(!(ms1 == ms2));
	assert(!(ms2 == ms1));

	std::cout << "Posso usare l'operatore = su due oMultiset non vuoti e cambiarne la dimensione: ms2 = ms1" << std::endl;
	ms2 = ms1;														//assegnamento di un oMultiset vuoto ad un oMultiset pieno
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == ms2.elementsCount());
	assert(ms1 == ms2);
	std::cout << "Svuoto un oMultiset con clear()" << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Posso anche usare la clear() su oMultiset appena svuotato, non succede nulla: " << std::endl;
	ms1.clear();
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	assert(ms1.elementsCount() == 0);
	std::cout << "Cambo gli elementi contenuti in ms1: " << std::endl;
	ms1.add(elemento8);
	ms1.add(elemento7);
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "L'assegnamento funziona anche quando il contenuto è completamente diverso: ms2 = ms1" << std::endl;
	ms2 = ms1;
	std::cout << "ms1: " << ms1 << std::endl <<  "ms2: " << ms2 << std::endl;
	std::cout << "Test metodo contains:" << std::endl;
	std::cout << "ms1.contains(" << elemento7 << ")? " << ms1.contains(elemento7) << 
				", ms1.contains(" << elemento8 << ")? " << ms1.contains(elemento8) << 
				", ms1.contains(" << elemento1 << ")? " << ms1.contains(elemento1) << std::endl;
	assert(ms1.contains(elemento7));
	assert(ms1.contains(elemento8));
	assert(!(ms1.contains(elemento1)));

	std::cout << "Test costruttore tramite iteratori num.1, creazione di due oMultiset con funtore Comp diverso:" << std::endl;
	ordMsetPoint3D ms5(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms5: " << ms5 << std::endl;
	oMultiset<typePoint3D, point_decresc, point_eql> ms8(arrayDatiMset, arrayDatiMset+5);
	std::cout << "ms8: " << ms8 << std::endl;
	std::cout << "ms5 == ms8? " << (ms5 == ms8) << ". E ms8 == ms5? " << (ms8 == ms5) << "." << std::endl;
	assert(ms5 == ms8);
	assert(ms8 == ms5);

	std::cout << "Test costruttore tramite iteratori num.2:" << std::endl;
	ordMsetPoint3D ms6(ms5.begin(), ms5.end());
	std::cout << "ms6: " << ms6 << std::endl;

	std::cout << "Test costruttore tramite iteratori num.3(da oMultiset vuoto):" << std::endl;
	ms5.clear();
	ordMsetPoint3D ms7(ms5.begin(), ms5.end());
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;
	std::cout << "Verifico l'assenza di condivisione di dati aggiungendo un elemento da ms7: " << std::endl;
	ms7.add(elemento6);
	std::cout << "ms5: " << ms5 << std::endl << "ms7: " << ms7 << std::endl;

	std::cout << "Test iteratori:" << std::endl;
	ordMsetPoint3DConst_iterator b,e;
	b = ms5.begin();
	e = ms5.end();
	std::cout << "Verifico che ms5.begin() == ms5.end(): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	e = ms7.end();
	std::cout << "Verifico che ms7.begin() != ms7.end(): " << (b != e) << std::endl;
	assert(b != e);
	std::cout << "Verifico che *b == ms7.begin(): " << (*b == *(ms7.begin())) << std::endl;
	assert(*b == *(ms7.begin()));
	std::cout << "Verifico che (++ms7.begin()) == ms7.end(): " << (++b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	std::cout << "Verifico che (ms7.begin()++) != ms7.end(): " << (b++ == e) << std::endl;
	assert(b == e);
	std::cout << "Verifico che ora il postincremento di b abbia funzionato, (b == e): " << (b == e) << std::endl;
	assert(b == e);
	b = ms7.begin();
	ordMsetPoint3DConst_iterator e2(b);									//copyconstructor dell'iteratore, ora b == e2 == ms7.begin()
	std::cout << "Verifico copy construcotr iteratore (b == e2): " << (b == e2) << std::endl;
	assert(b == e2);
	std::cout << "Se incremento e2 otterrò un iteratore che punta allo stesso indirizzo di ms7.end() == e: " << (++e2 == e) << std::endl;
	assert(e2 == e);
	
	std::cout << std::endl << std::endl << std::endl;
}



//======================================== MAIN ========================================


int main(int argc, char *argv[]) {
	std::cout << "==================== INIZIO MAIN ====================" << std::endl << std::endl << std::endl;

	#ifndef NDEBUG
	std::cout << "Modelità debug attiva." << std::endl;
	#endif

	ordMsetInt msInt;
	ordMsetString msString;
	ordMsetPoint3D msSPoint3D;
	testMultisetInt(msInt);
	testConstMultisetInt(msInt);
	testMultisetString(msString);
	testMultisetPoint3D(msSPoint3D);

	std::cout << std::endl << std::endl << std::endl << "==================== FINE MAIN ====================" << std::endl;
	return 0;
}
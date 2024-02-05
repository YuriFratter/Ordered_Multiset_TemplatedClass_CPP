#ifndef MULTISET_H
#define MULTISET_H
#include <iostream>		//std::ostream
#include <stdexcept>	// std::runtime_error
#include <iterator>		// std::forward_iterator_tag
#include <cstddef> 		// std::ptrdiff_t
#include "elementNotFound.h"


/**
 * @brief classe oMultiset di oggetti ordinati
 *
 * Questa classe container crea un Multiset Ordinato di elementi generici T. Un oMultiset si può vedere come un insieme di elementi
 * il quale a differenza dei classici insiemi ammette la coesistenza di più elementi uguali fra loro.
 * L'uguaglianza tra due elementi è determinata grazie al parametro templato Eql, usato dalla classe come funtore di uguaglianza; 
 * esso deve essere definito dall'utente in modo da permettere il confronto anche fra oggetti custom.
 * Trattandosi di un Multiset ORDINATO si è reso necessario l'utilizzo di un terzo parametro templato: Comp è utilizzato come
 * operatore di confronto per stabilire l'ordine che assumeranno i dati salvati nell'oMultiset.
 * 
 * @tparam	T Tipo di dato che verrà salvato nella struct node della classe container.
 * @tparam	Comp Funtore per il confronto degli elementi T, utile a determinare l'ordine di essi.
 * @tparam	Eql Funtore per l'uguaglianza tra elementi T, utile a determinare quando si equivalgono tra loro.
 */
template<typename T, typename Comp, typename Eql>
class oMultiset {

public:

	typedef unsigned int size_type;					///< typedef per gli interi
	typedef T value_type;							///< typedef per dato templato T

private:

	/**
	 * @brief classe privata per salvare dati
	 *
	 * Questa struttura è stata messa privata perché nonostante potrebbe essere potenzialmente riutilizzata 
	 * non è stata pensata per questo scopo ma solo ad utilizzo interno e privato della classe container oMultiset.
	 */
	struct node{
		value_type element;							///< elemento generico T salvato nella struct
		size_type occurrence;						///< numero di occorrenze dell'elemento nella stessa struct node

		/**
		 * @brief costruttore di default. (METODO FONDAMENTALE)
		 * 
		 * @post element == valore di default dato dal costruttore di default del tipo value_type(T).
		 * @post occurrence == 0
		 */
		node() : element(value_type()), occurrence(0) {}

		/**
		 * @brief Copy Constructor (METODO FONDAMENTALE)
		 * Costruttore che inizializza un nuovo node copiando i dati di un altro node.
		 * 
		 * @param other altro dato di tipo node che verrà copiato nel nodo corrente.
		 * 
		 * @post element == other.element
		 * @post occurrence == other.occurrence
		 */
		node(const node &other) : element(other.element), occurrence(other.occurrence) {}


		/**
		 * @brief Operatore di assegnamento. (METODO FONDAMENTALE)
		 *
		 * @param other altro dato di tipo node.
		 * 
		 * @post element == other.element
		 * @post occurrence == other.occurrence
		 */
		node &operator=(const node &other) { 
			node tmp(other);
			std::swap(element, tmp.element);
			std::swap(occurrence, tmp.occurrence);
			return *this;
		}


		/**
		 * @brief Distruttore. (METODO FONDAMENTALE)
		 */
		~node(){}


		/**
		 * @brief costruttore secondario.
		 * 
		 * @param element valore di tipo value_type (T) da salvare nella nostra struttura dati.
		 *
		 * @post element == element
		 * @post occurrence == 1
		 */
		explicit node(const value_type &element) : element(element), occurrence(1) {}
	};


	/**
	 * @brief Costruttore secondario privato della classe oMultiset.
	 * 
	 * @param size Dimensione da dare all'array.
	 * 
	 * @post _arraySize == size
	 * @throw std::bad_alloc nel caso in cui l'istanziazione in memoria fallisca.
	 */
	explicit oMultiset(size_type size) : _array(nullptr), _arraySize(0) {
		if(size > 0) {
			_array = new node[size];
			_arraySize = size;
		}
	}

	/**
	 * @brief funzione swap per scambiare i dati membro con un altro oMultiset.
	 * 
	 * Questa funzione utilizza la std::swap su ogni dato della classe oMultiset per scambiare
	 * a tutti gli effetti il contenuto di un oMultiset con il contenuto di un altro.
	 * 
	 * @param other Multiset con il quale si vogliono scambiare i dati membro.
	 */
	void swap(oMultiset <value_type, Comp, Eql> &other){
		std::swap(_array, other._array);
		std::swap(_arraySize, other._arraySize);
		std::swap(_compare, other._compare);
		std::swap(_equals, other._equals);
	}

public:

	/**
	 * @brief Costruttore di default. (METODO FONDAMENTALE)
	 *
	 * Inizializza un oMultiset ponendo il puntatore a nullptr e la size a 0,
	 * che corrisponde ad uno stato vuoto coerente.
	 */
	oMultiset() : _array(nullptr), _arraySize(0) {}

	/**
	 * @brief Copy constructor. (METODO FONDAMENTALE)
	 * 
	 * 
	 *
	 * @param src oMultiset da cui copiare tutti i dati membro
	 * @throw std::bad_alloc in questo metodo può fallire sia l'istanziazione di memoria sia l'assegnamento nel ciclo for
	 * in entrambi i casi il chiamante riceve un errore di fallita istanziazione std::bad_alloc.
	 */
	oMultiset(const oMultiset &src) : _array(nullptr), _arraySize(0) {
		_array = new node[src._arraySize];
		try {
			for(size_type i=0; i<src._arraySize; ++i)
				_array[i] = src._array[i];
			_arraySize = src._arraySize;
		} catch (...) {
			clear();
			throw;
		}
	}

	/**
	 * @brief Operatore di assegnamento. (METODO FONDAMENTALE)
	 * 
	 * @param other oMultiset da cui copiare tutti i dati membro.
	 * @return oMultiset& reference al oMultiset this.
	 */
	oMultiset& operator=(const oMultiset &other) {
		if(this!=&other) {
			oMultiset tmp(other);
			swap(tmp);    
		}
		return *this;
	}

	/**
	 * @brief Distruttore. (METODO FONDAMENTALE)
	 */
	~oMultiset() {
		delete[] _array;
	}


	/**
	 * @brief Conta occorrenze di un elemento.
	 *
	 * Metodo che conta le occorrenze di un elemento all'interno dell'oMultiset.
	 * 
	 * @param element istanza di elemento da cercare all'interno dell'oMultiset.
	 * @return occorrenze di un elemento: 0 se assente >0 altrimenti.
	 */
	size_type occurrsCount(const value_type &element) const {
		size_type res = 0;
		for(size_type i=0; i<_arraySize; ++i){
			if(_equals(element, _array[i].element))
				res = _array[i].occurrence;
		}
		return res;
	}


	/**
	 * @brief Conta gli elementi totali all'interno dell'oMultiset.
	 * 
	 * @return Conta totale degli elementi (occorrenze comprese) 
	 */
	size_type elementsCount() const {
		size_type count = 0;
		for(size_type i=0; i<_arraySize; ++i)
			count += occurrsCount(_array[i].element);
		return count;
	}

	/**
	 * @brief Dice se un elemento è contenuto in un oMultiset.
	 * 
	 * @param element elemento da cercare all'interno dell'oMultiset.
	 * @return true se l'elemento è presente nell'oMultiset.
	 * @return false se l'elemento non è presente nell'oMultiset.
	 */
	bool contains(const value_type &element) const {
		for(size_type i=0; i<_arraySize; ++i) 
			if(_equals(element, _array[i].element))
				return true;
		return false;
	}


	/**
	 * @brief Metodo che si occupa di inserire un elemento all'interno dell'oMultiset nel giusto ordine.
	 *
	 * Questo metodo si occupa di aggiungere elementi di tipo value_type (T) 
	 * all'interno dell'oMultiset nel giusto ordine.
	 * Sono state utilizzate due variabili temporanee, una di queste tmpMs2 
	 * utilizza il costruttore secondario privato
	 * per istanziare un oMultiset in base alla dimensione necessaria (nel caso in cui si 
	 * incrementerà la dimensione dell'oMultiset).
	 * All'interno del metodo si distinguono 3 parti (le prime due lavorano su tmpMs1 e 
	 * la seconda tmpMs2 in tutti e 3 i casi
	 * si effettuano tutte le operazioni necessarie sulle variabili temporanee ed infine 
	 * viene chiamata la swap per scambiare
	 * i dati membro tra la variabile temporanea utilizzata e l'oggetto this), che 
	 * corrispondono alle 3 casistiche:
	 * - l'oMultiset è vuoto: non bisogna seguire nessun ordine, basta usare tmpMs2 
	 * 		di dimensione +1 rispetto al corrente (0+1 = 1) e 
	 * 		sovrascrivere il valore nell'unico elemento presente in esso;
	 * - l'oMultiset contiene già l'elemento che si vuole inserire: in questo caso basterà 
	 * 		incrementare la sua occorrenza di 1;
	 * - l'elemento non è presente nell'oMultiset: in questo caso i dati vengono copiati 
	 * 		a parità di posizione da this a tmpMs2 fino al momento
	 * 		in cui viene trovato il posto dove inserire l'elemento, si aggiunge l'elemento e 
	 * 		da lì in poi i dati di this->_array[i-1] vengono copiati in tmpMs2._array[i]
	 * 
	 * @param value elemento da aggiungere al oMultiset.
	 * @throw std::bad_alloc nel caso l'allocazione in memoria fallisca.
	 */
	void add(const value_type &value) {
		oMultiset tmpMs(*this);
		oMultiset tmpMs2(_arraySize+1);
		node nodo(value);
		//caso in cui l'oMultiset è vuoto.
		if(_arraySize == 0){
			tmpMs2._array[0] = nodo;
			tmpMs2._arraySize = 1;
			swap(tmpMs2);
			return;
		} 
		//caso in cui il valore è già presente.
		else if(contains(value)){
			for(size_type i=0; i<_arraySize; ++i) 
				if(_equals(value, _array[i].element))
					++tmpMs._array[i].occurrence;
			swap(tmpMs);
			return;
		}
		//casi in cui l'elemento non è presente e va incrementato l'array
		else {
			//caso particolare in cui value va in testa
			if(_compare (value, _array[0].element)){
				tmpMs2._array[0] = nodo;
				for(size_type i=1; i<=_arraySize; ++i) {
					tmpMs2._array[i] = _array[i-1];
				}
			} 
			//caso medio in cui value va in centro o alla fine
			else {
				bool inserito = false;
				for(size_type i=0; i<=_arraySize; ++i) {
					if(inserito == false && i<_arraySize) {
						if(_compare(value, _array[i].element)){	
							tmpMs2._array[i] = nodo;
							inserito = true;
						} else {
							tmpMs2._array[i] = _array[i];
						}
					} else {
						if(inserito == false){
							tmpMs2._array[i] = nodo;
							inserito = true;
						} else{
							tmpMs2._array[i] = _array[i-1];
						}
					}
				}
			}
		}
		swap(tmpMs2);
	}

	/**
	 * @brief Rimuove un elemento dall'oMultiset.
	 *
	 * Tutte le operazioni di questo metodo vengono prima svolte su 
	 * oMultiset temporanei ed infine le quali alla fine (ad
	 * operazioni eseguite) tramite la funzione swap scambiano 
	 * i dati con l'oMultiset corrente.
	 * I principali casi possibili di questo metodo sono due e sono 
	 * riconoscibili grazie al funtore di uguaglianza Eql:
	 * - L'elemento da rimuovere non è presente nell'oMultiset: 
	 * 		in questo caso viene lanciata un'eccezione 
	 * 		custom che notifica l'utente che l'elemento non è presente 
	 * 		nell'oMultiset (ovviamente con oMultiset vuoto si rientrerà sempre in questo caso);
	 * - L'elemento da rimuovere è presente nell'oMultiset:
	 * 		- L'elemento è presente nell'oMultiset (occorrenza == 1): in questo caso 
	 * 				viene rimosso e tutti gli altri elementi vengono
	 * 				copiati nell'array tmpMs2 con size decrementata di 1;
	 * 		- L'elemento è presente nell'oMultiset (occorrenza > 1): in questo caso 
	 * 				viene semplicemente decrementata l'occorrenza;
	 *
	 * @param value elemento da rimuovere dall'oMultiset.
	 * @throw elementNotFound eccezione che eredita da std::runtime_error.
	 */
	void remove(const value_type &value) {
		if(!contains(value)){
			//L'elemento non è contenuto nell'oMultiset.
			throw elementNotFound("Elemento non trovato: oMultiset vuoto o elemento non presente nell'oMultiset.");
		}
		//L'elemento è contenuto nell'oMultiset.
		oMultiset tmpMs(*this);
		oMultiset tmpMs2(_arraySize-1);
		bool rimosso = false;
		bool decrementato = false;
		for(size_type i=0; i<_arraySize; ++i) {
			if(!rimosso && !decrementato){
				if(_equals(value, _array[i].element)){
					if(_array[i].occurrence == 1)
						rimosso = true;
					else{
						--tmpMs._array[i].occurrence;
						decrementato = true;
					}	
				} else {
					tmpMs2._array[i] = _array[i];
				}
			} else {
				tmpMs2._array[i-1] = _array[i];
			}
		}
		if(rimosso == false)
			swap(tmpMs);
		else
			swap(tmpMs2);
	}

	/**
	 * @brief Svuota un oMultiset.
	 *
	 * Questo metodo itera una remove sul primo elemento dell'oMultiset finché l'oMultiset ha almeno un elemento.
	 * Quando l'oMultiset non ha più elementi il metodo termina (se l'oMultiset non aveva elementi dall'inizio 
	 * il metodo termina senza eseguire nessuna operazione)
	 *
	 * @post this->elementsCount() == 0 
	 */
	void clear(){
		while(_arraySize > 0)
			remove(_array[0].element);
	}


	/**
	 * @brief Ridefinizione globale dell'operatore di stream.
	 *
	 * Permette di poter stampare un oggetto oMultiset direttamente tramite l'operatore di stream in una forma che 
	 * ne rappresenta la struttura interna. È stata definita all'interno della classe oMultiset per far dedurre automaticamente
	 * i parametri templati dall'interno della classe; è stata messa friend per poter accedere ai dati privati
	 * di oMultiset.
	 * 
	 * @param os stream di output.
	 * @param mSet oMultiset da aggiungere allo stream per la stampa.
	 * @return stream di output std::ostream.
	 */
	friend std::ostream &operator<<(std::ostream &os, const oMultiset &mSet) {
		std::cout<< "{";
		for(size_type i=0; i<mSet._arraySize; ++i) {
			if(i<mSet._arraySize-1)
				std::cout<< "<" << mSet._array[i].element << ", " << mSet._array[i].occurrence << ">, ";
			else
				std::cout<< "<" << mSet._array[i].element << ", " << mSet._array[i].occurrence << ">";
		}
		std::cout<< "}";
		return os;
	} 

	/**
	 * @brief Costruttore tramite iteratori
	 * 
	 * Costruttore che crea un oMultiset a partire da una sequenza delimitata da un iteratore
	 * di inizio ed un iteratore di fine.
	 * 
	 * @param a iteratore di inizio.
	 * @param b iteratore di fine.
	 * @throw std::bad_alloc nel caso l'allocazione in memoria fallisca.
	 */
	template<typename Iter>
	oMultiset(Iter a, Iter b) : _array(nullptr), _arraySize(0) {
		oMultiset tmpMs;
		for( ; a!=b; ++a){
			tmpMs.add(static_cast<value_type>(*a));
		}	
		swap(tmpMs);
	}

	/**
	 * @brief Operatore di confronto tra oMultiset.
	 * 
	 * La ridefinizione di questo operatore permette di confrontare due oMultiset che possono differire
	 * di funtore di confronto, questo ad indicare che non importa il loro ordine, gli unici parametri 
	 * rilevanti per il confronto sono i valori contenuti (devono essere gli stessi) e devono avere le
	 * stesse occorrenze. L'uguaglianza tra elementi templati T è possibile grazie al funtore di 
	 * eguaglianza Eql.
	 *
	 * @tparam Comp2 Funtore di ordinamento dell'oMultiset da confrontare al corrente, può essere anche diverso.
	 * @param other Multiset da confrontare al oMultiset corrente.
	 * @return true Se i due oMultiset sono uguali in valori contenuti ed occorrenze.
	 * @return false Se i due oMultiset non sono uguali in valori contenuti ed occorrenze. 
	 */
	template<typename Comp2>
	bool operator==(const oMultiset <T,Comp2,Eql> &other) const {
		oMultiset tmpMs(*this);
		typename oMultiset <T,Comp2,Eql>::oMultiset tmpOtherMs(other);
		if(tmpMs.elementsCount() == tmpOtherMs.elementsCount()){
			while(tmpMs.elementsCount() > 0) {
				if(tmpOtherMs.contains(tmpMs._array[0].element)){
					tmpOtherMs.remove(tmpMs._array[0].element);
					tmpMs.remove(tmpMs._array[0].element);
				} else {
					return false;
				}
			}
		} else
			return false;
		return true;
	}



	//////////////////////////////////////////////ITERATORI////////////////////////////////////////////////////


	/**
	* @brief Iteratore di sola lettura.
	*/
	class const_iterator {

	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;


		//Costruttore di default. (METODO FONDAMENTALE)
		const_iterator() : _ptr(nullptr), _occurrenceIter(0) {}
		

		//Copy constructor. (METODO FONDAMENTALE)
		const_iterator(const const_iterator &other) : _ptr(other._ptr), _occurrenceIter(other._occurrenceIter) {}


		//Operatore di assegnamento. (METODO FONDAMENTALE)
		const_iterator& operator=(const const_iterator &other) {
			if(other != nullptr){
				_ptr = other._ptr;
				_occurrenceIter = other._occurrenceIter;
			}
			return *this;
		}


		//Distruttore (METODO FONDAMENTALE)
		~const_iterator() {}


		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return _ptr->element;
		}


		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(_ptr->element);
		}
		

		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			if(_occurrenceIter == 0){
				_occurrenceIter = _ptr->occurrence;
			}
			if(_occurrenceIter >= 1) {
				--_occurrenceIter;
				if(_occurrenceIter == 0)
					++_ptr;
			}
			return tmp;
		}


		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			if(_occurrenceIter == 0){
				_occurrenceIter = _ptr->occurrence;
			}
			if(_occurrenceIter >= 1) {
				--_occurrenceIter;
				if(_occurrenceIter == 0)
					++_ptr;
			}
			return *this;
		}


		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return (_ptr == other._ptr && _occurrenceIter == other._occurrenceIter);
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return !(*this == other);
		}


	private:

		//Dati membro
		const node *_ptr;
		size_type _occurrenceIter;

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class oMultiset;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(const node *nodo) : _ptr(nodo), _occurrenceIter(0) {}
	}; // classe const_iterator
	

	/**
	 * @brief Creazione const_iterator di inizio sequenza.
	 * 
	 * @return Iteratore di inizio sequenza oMultiset.
	 */
	const_iterator begin() const {
		return const_iterator(_array);
	}
	

	/**
	 * @brief Creazione const_iterator di fine sequenza.
	 * 
	 * @return Iteratore di fine sequenza oMultiset.
	 */
	const_iterator end() const {
		return const_iterator(_array + _arraySize);
	}
	

private: 


	node *_array;									///< Array dei valori salvati dentro l'oMultiset
	size_type _arraySize;							///< Dimensione dell'array (unsigned int)
	Comp _compare;									///< Funtore di confronto per l'ordine degli elementi.
	Eql _equals;									///< Funtore di uguaglianza per equiparare degli elementi.
	
	};


#endif
//
// Copyright Daan de Lange 2023
//

// A very simple linked list implementation
// Inspired by Mosra/Corrade's LinkedList
// About 10 times faster then a std::map.
// Relatively slow add/remove but very fast to iterate.
// No ownership, just referencing pointers.

// Fwd declaration needed
template<class T_KEY, class T_DATA>
class LinkedList;

// List Items
template<class T_KEY, class T_DATA>
struct LinkedListItem {
		friend class LinkedList<T_KEY, T_DATA>;

	public:
		LinkedListItem<T_KEY, T_DATA>* const getNext(){
			return next;
		};
		T_KEY* const key;
		T_DATA* const data;
		LinkedListItem(T_KEY* _key, T_DATA* _data, LinkedListItem<T_KEY, T_DATA>* _next = nullptr) :
			next(_next), key(_key), data(_data){}

	private:
		LinkedListItem<T_KEY, T_DATA>* next = nullptr;
};

// THE list
template<class T_KEY, class T_DATA>
class LinkedList {
	public:
		LinkedList() : first(nullptr) {}
		~LinkedList(){
			LinkedListItem<T_KEY, T_DATA>* item = first;
			while(item){
				LinkedListItem<T_KEY, T_DATA>* next = item->next;
				delete item;
				item->next = nullptr;
				item = next;
			}
			first = nullptr;
		}

		// Disable copy
		LinkedList<T_KEY,T_DATA>& operator=(const LinkedList<T_KEY, T_DATA>&) = delete;

		void add(T_KEY* _glfwWin, T_DATA* _engine){
			LinkedListItem<T_KEY, T_DATA>* last = first;
			LinkedListItem<T_KEY, T_DATA>* newEntry = new LinkedListItem<T_KEY, T_DATA>(_glfwWin, _engine);
			// First entry ?
			if(last==nullptr){
				first = newEntry;
				return;
			}
			while(last){
				if(last->getNext()){
					last = last->getNext();
				}
				else {
					break;
				}
			}
			last->next = newEntry;
		}
		void remove(T_KEY* _key){
			if(_key==nullptr) return;
			LinkedListItem<T_KEY, T_DATA>* item = first;
			while(item){
				// Replace first with next if it's the first entry
				if(item == first && item->key == _key){
					first = item->next;
					return;
				}
				// Replace next of previous-to-item
				if(item->next != nullptr && item->next->key == _key){
					item->next = item->next->next;
					return;
				}
				// Incremment
				item = item->next;
			}
			return;
		}

		bool hasKey(T_KEY* _key){
			return findData(_key) != nullptr;
		}

		T_DATA* findData(T_KEY* _key){
			if(_key==nullptr) return nullptr;
			LinkedListItem<T_KEY, T_DATA>* item = first;
			while(item){
				if(item->key == _key) return item->data;
				item = item->getNext();
			}
			return nullptr;
		}
		T_KEY* findKey(T_DATA* _data){
			if(_data==nullptr) return nullptr;
			LinkedListItem<T_KEY, T_DATA>* item = first;
			while(item){
				if(item->data == _data) return item->key;
				item = item->getNext();
			}
			return nullptr;
		}
		LinkedListItem<T_KEY, T_DATA>* const getFirst(){
			return first;
		}
	private:
		LinkedListItem<T_KEY, T_DATA>* first = nullptr;
};

# cpp-simple-vector
## Additional languages: [Русский](Russian/README.md)

## Realisation of sequenced container - vector
### simple_vector.h
Developed template class SimpleVector. This is a highly simplified analog of the STL vector container, with a similar structure and functionality.

Main features realised:
*	Constructors.
    * By default. Creates an empty container with zero capacity.
    * Constructor that creates a container of a given size.
    * Constructor from std::initializer_list.
*	Copy constructor. 
*	Assignment operator. 
*	Operators **==** and **!=**.
*	Operators **<**, **>**, **<=**, **>=**, performing lexicographic comparison of the contents of two containers.
*	Operator **[]** returns a reference to the element at specified location.
*	Move semantics support.
*	Methods:
	* **GetSize** returns the number of elements in the container.
	* **GetCapacity** returns capacity of the currently allocated storage.
	* **IsEmpty**, returns true if the container is empty, false otherwise
	* **At** returns reference to the requested element.
	* **Clear** erases all elements from the container. Leaves the capacity of the containter unchanged. 
	* **Resize** resizes the container. 
	* **begin**, **end**, **cbegin** and **cend**, returns an iterator to the first(last for end/cend) element of the container.
	* **PushBack**, appends the given element value to the end of the container. 
	* **PopBack**, removes the last element of the container.
	* **Insert**, inserts element at the specified location in the container.
	* **Erase**, erases the specified element from the container. 
	* **swap**, exchanges the contents of the container with those of other.
	* **Reserve**, increases the capacity of the container.

### array_ptr.h
Developed a template class ArrayPtr, that plays a role of a smart pointer to an array in dynamic memory.

Main features realised:
*	Array deleting if smart pointer is destroyed.
*	Constructors: 
    * By default.	
    * From a pointer to an existing array. Makes a new array of specified size.
*	Accessing an array element by index.
*	Disabled copy and assignment operations.
*	**swap** exchanges the contents with other ArrayPtr object.
*	**Release**, releases the ownership of the managed array. Returns raw pointer to it.
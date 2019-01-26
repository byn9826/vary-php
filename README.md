# Vary-PHP
Various PHP classes in C

Installation
-
```
sudo apt-get install php7.2-cli
git clone https://github.com/php/php-src.git
cd ./ext
git clone https://github.com/byn9826/vary-php.git
cd ./vary
phpize
./configure
sudo make
sudo make install
[set extension=vary.so in php.ini]
php ./tests/index.php
```
[Testing & Examples](https://github.com/byn9826/vary-php/tree/master/tests)
```
sudo apt-get install valgrind
npm install
npm run test
npm run valgrind
```

Classes for Data Structure
--
ArrayList: Array type Collections
```
$array = new \Vary\ArrayList();
$array->push(2); // true
$array->unshift(1); // true
$array->push(3); // true
$array->value(); // [1, 2, 3]
$array->removeIndex(1); // 2
$array->pop(); // 3
$array->shift(); // 1
$array->size(); // 0
$array = new \Vary\ArrayList(['ab', 0, -1, 'ab', 0, -1, 100]);
$array->size(); // 7
$array->indexOf('ab'); // 0
$array->lastIndexOf('ab'); // 3
```
Stack: Last-In-First-Out type ArrayList
```
$stack = new \Vary\Stack();
$stack->push(1); // true
$stack->pop(); // 1
$stack->size(); // 2
$stack = new \Vary\Stack([1, 2]);
$stack->value(); // [1, 2]
$stack->pop(); // 2
```
Queue: First-In-First-Out type ArrayList
```
$queue = new \Vary\Queue();
$queue->push(1); // true
$queue->shift(); // 1
$queue->value(); // []
$queue = new \Vary\Queue([1, 2]);
$queue->shift(); // 1
$queue->size(); // 1
```
Deque: Double-Ended type ArrayList
```
$deque = new \Vary\Deque();
$deque->push(1); // true
$deque->unshift(2); // true
$deque->value(); // [2, 1]
$deque->shift(); // 2
$deque->pop(); // 1
$deque->size(); // 0
```
OrderedList: Ordered ArrayList
```
$ordered_list = new \Vary\OrderedList();
foreach([1, 0, 3, -1, 50] as $value) {
  $ordered_list->add($value);
}
$ordered_list->size(); // 5
$ordered_list->shift(); // -1
$ordered_list->shift(); // 0
$ordered_list->pop(); // 50
$ordered_list->pop(); // 3
$ordered_list->value(); // [1]
$ordered_list->add(0); // 0
$ordered_list->add(2); // 2
$ordered_list->add(1); // false
$ordered_list->removeIndex(0); // 0
$ordered_list->removeIndex(3); // null
$ordered_list->remove(0); // false
$ordered_list->remove(2); // 1
$ordered_list->indexOf(1); // 0
$ordered_list->indexOf(2); // false
$ordered_list->add(2); // 1
$ordered_list->indexOf(2); // 1
```

Algorithm Class
--
Binary Search
```
\Vary\Algorithm::binarySearch([-1, 1, 2], 2); // ['index': 2, 'exist': true]

\Vary\Algorithm::binarySearch([-1, 1, 2], 3); // ['index': 2, 'exist': false]

\Vary\Algorithm::binarySearch([-1, 1, 2], -1); // ['index': 0, 'exist': true]

\Vary\Algorithm::binarySearch([-1, 1, 2], -2); // ['index': 0, 'exist': false]

\Vary\Algorithm::binarySearch([-1, 1, 2], 1); // ['index': 1, 'exist': true]
```
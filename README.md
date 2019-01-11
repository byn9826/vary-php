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
Stack
```
$stack1 = new \Vary\Stack();
$stack1->push(1); // true
$stack1->pop(); // 1
$stack2 = new \Vary\Stack([1, 2]);
$stack2->size(); // 2
$stack2->pop(); // 2
```
Queue
```
$queue1 = new \Vary\Queue();
$queue1->enqueue(1); // true
$queue1->dequeue(); // 1
$queue2 = new \Vary\Queue([1, 2]);
$queue2->dequeue(); // 1
$queue2->size(); // 1
```
Deque
```
$deque1 = new \Vary\Deque();
$deque1->addRear(1); // true
$deque1->addFront(2); // true
$deque1->removeFront(); // 2
$deque1->removeRear(); // 1
$deque1->size(); // 0
```

Algorithm Class
--
Binary Search
```
$binarySearch1 = \Vary\Algorithm::binarySearch([-1, 1, 2], 2); // ['index': 2, 'exist': true]

$binarySearch2 = \Vary\Algorithm::binarySearch([-1, 1, 2], 3); // ['index': 2, 'exist': false]

$binarySearch3 = \Vary\Algorithm::binarySearch([-1, 1, 2], -1); // ['index': 0, 'exist': true]

$binarySearch4 = \Vary\Algorithm::binarySearch([-1, 1, 2], -2); // ['index': 0, 'exist': false]

$binarySearch5 = \Vary\Algorithm::binarySearch([-1, 1, 2], 1); // ['index': 1, 'exist': true]
```
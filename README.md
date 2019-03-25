# VARY-PHP
Various PHP classes in C  
Supported Version: php7.2  

Installation
-
```
sudo apt-get install php7.2-cli php7.2-dev valgrind
git clone https://github.com/php/php-src.git
cd ./ext
git checkout PHP-7.2.15
git clone https://github.com/byn9826/vary-php.git
cd ./vary-php
phpize
./configure
sudo make
sudo make install
sudo nano /etc/php/7.2/cli/php.ini
[Add extension=vary.so]
php ./tests/index.php
```
[Testing & Examples](https://github.com/byn9826/vary-php/tree/master/tests)
```
npm install
npm run test
npm run valgrind
```

Classes
--
<b>Data Structures Classes:</b>  
ArrayList, Stack, Queue, Deque, OrderedList  
<b>Algorithm Functions:</b>  
binarySearch, shellShort

ArrayList: Array Type Collections
--
constructor(), value(), length(), index()
```
$array = new \Vary\ArrayList();
$array->value(); // []
$array->length(); // 0
$array = new \Vary\ArrayList([1, 2, 3]);
$array->value(); // [1, 2, 3]
$array->length(); // 3
$array->index(1); // 2
$array->index(3); // null
```
setValue()
```
$array = new \Vary\ArrayList();
$array->setValue([1, 2, 3]);
$array->value(); // [1, 2, 3]
```
removeIndex()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->removeIndex(1); // 2
$array->value(); // [1, 3]
```
push()
```
$array = new \Vary\ArrayList([1, 2]);
$array->push(3); // true
$array->value(); // [1, 2, 3]
```
pop()
```
$array = new \Vary\ArrayList([1, 2]);
$array->pop(); // 2
$array->value(); // [1]
```
unshift()
```
$array = new \Vary\ArrayList([2, 3]);
$array->unshift(1); // true
$array->value(); // [1, 2, 3]
```
shift()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->shift(); // 1
$array->value(); // [2, 3]
```
indexOf()
```
$array = new \Vary\ArrayList([1, 2, 1]);
$array->indexOf(1); // 0
$array->indexOf(3); // -1
```
lastIndexOf()
```
$array = new \Vary\ArrayList([1, 2, 1]);
$array->lastIndexOf(1); // 2
$array->lastIndexOf(3); // -1
```
includes()
```
$array = new \Vary\ArrayList([1, 2]);
$array->includes(1); // true
$array->includes(0); // false
```
sort()
```
$array = new \Vary\ArrayList([-1, 1, 0]);
$array->sort();
$array->value(); // [-1, 0, 1]
$array->sort(function($a, $b) { return $a > $b; });
$array->value(); // [1, 0, -1]
```
reverse()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->reverse();
$array->value(); // [3, 2, 1]
```
concat()
```
$array = new \Vary\ArrayList([1, 2]);
$array->concat([3, 4, 5]); // [1, 2, 3, 4, 5]
$array->value(); // [1, 2]
```
every()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->every(function($a) { return $a > 0; }) // TRUE
$array->every(function($a) { return $a > 1; }) // FALSE
```
some()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->some(function($a) { return $a === 0; }) // FALSE
$array->some(function($a) { return $a === 1; }) // TRUE
```
map()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->map(function($a) { return $a * $a; }) // [1, 4, 9]
$array->value(); // [1, 2, 3]
```
forEach()
```
$holder = new \Vary\ArrayList();
$array = new \Vary\ArrayList([1, 2, 3]);
$array->forEach(function($a) use($holder) { $holder->push($a * $a); });
$array->value(); // [1, 2, 3]
$holder->value(); // [1, 4, 9]
```
fill()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->fill(1);
$array->value(); // [1, 1, 1]
$array->fill(2, 1);
$array->value(); // [1, 2, 2]
$array->fill(3, 1, 2);
$array->value(); // [1, 3, 1]
```
reduce()
```
$array = new \Vary\ArrayList([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);
$array->reduce(function($a, $b) { return $a + $b; }); // 55
$array->reduce(function($a, $b) { return $a + $b; }, 5); // 60
```
filter()
```
$array = new \Vary\ArrayList([1, 2, 3]);
$array->filter(function($a) { return $a > 1; }); // [2, 3]
$array->value(); // [1, 2, 3]
```
find()
```
$array = new \Vary\ArrayList([
  ['value' => 0], ['value' => '1'], ['value' => '22']
]);
$array->find(function($a) { return $a['value'] === '1'; }); // ['value' => '1']
$array->find(function($a) { return $a['value'] === '333'; }); // null
```
findIndex()
```
$array = new \Vary\ArrayList([
  ['value' => 0], ['value' => '1'], ['value' => '22']
]);
$array->findIndex(function($a) { return $a['value'] === '1'; }); // 1
$array->findIndex(function($a) { return $a['value'] === '333'; }); // -1
```
splice()
```
$array = new \Vary\ArrayList([1, 2, 3, 4]);
$array->splice(1, 2);
$array->value(); // [1, 4]
$array->splice(1, 0, 2);
$array->value(); // [1, 2, 4]
```
slice()
```
$array = new \Vary\ArrayList([1, 2, 3, 4]);
$array->slice(1); // [2, 3, 4]
$array->slice(1, 3); // 2, 3
```

Stack: Last-In-First-Out Type ArrayList
--
constructor(), value(), length(), index()
```
$stack = new \Vary\Stack();
$stack->value(); // []
$stack->length(); // 0
$stack = new \Vary\Stack([1, 2, 3]);
$stack->value(); // [1, 2, 3]
$stack->length(); // 3
$stack->index(1); // 2
```
push()
```
$stack = new \Vary\Stack([1]);
$stack->push(2); // true
$stack->value(); // [1, 2]
```
pop()
```
$stack = new \Vary\Stack([1, 2, 3]);
$stack->pop(); // 3
$stack->value(); // [1, 2]
```

Queue: First-In-First-Out Type ArrayList
--
constructor(), value(), length(), index()
```
$queue = new \Vary\Queue();
$queue->value(); // []
$queue->length(); // 0
$queue = new \Vary\Queue([1, 2, 3]);
$queue->value(); // [1, 2, 3]
$queue->length(); // 3
$queue->index(1); // 2
```
push()
```
$queue = new \Vary\Queue([1]);
$queue->push(2); // true
$queue->value(); // [1, 2]
```
shift()
```
$queue = new \Vary\Queue([1, 2, 3]);
$queue->shift();
$queue->value(); // [2, 3]
```

Deque: Double-Ended Type ArrayList
--
constructor(), value(), length(), index()
```
$deque = new \Vary\Deque();
$deque->value(); // []
$deque->length(); // 0
$deque = new \Vary\Deque([1, 2, 3]);
$deque->value(); // [1, 2, 3]
$deque->length(); // 3
$queue->index(1); // 2
```
push()
```
$deque = new \Vary\Deque([1]);
$deque->push(2); // true
$deque->value(); // [1, 2]
```
pop()
```
$deque = new \Vary\Deque([1, 2, 3]);
$deque->pop(); // 3
$deque->value(); // [1, 2]
```
unshift()
```
$deque = new \Vary\Deque([2]);
$deque->unshift(1); // true
$deque->value(); // [1, 2]
```
shift()
```
$deque = new \Vary\Deque([1, 2, 3]);
$deque->shift(); // 1
$deque->value(); // [2, 3]
```

OrderedList: Ordered ArrayList of Unique Numbers
--
constructor(), value(), length(), index()
```
$ordered_list = new \Vary\OrderedList();
$ordered_list->value(); // []
$ordered_list->length(); // 0
$ordered_list->add(1);
$ordered_list->index(0); // 1
```
add()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 4, 1] as $value) { $ordered_list->add($value); }
$ordered_list->add(3); // 2
$ordered_list->add(3); // false
$ordered_list->value(); // [1, 2, 3, 4]
```
remove()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 3, 1] as $value) { $ordered_list->add($value); }
$ordered_list->remove(2); // 1
$ordered_list->remove(2); // false
$ordered_list->value(); // [1, 3]
```
removeIndex()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 3, 1] as $value) { $ordered_list->add($value); }
$ordered_list->removeIndex(1); // 2
$ordered_list->value(); // [1, 3]
```
pop()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 3, 1] as $value) { $ordered_list->add($value); }
$ordered_list->pop(); // 3
$ordered_list->value(); // [1, 2]
```
shift()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 3, 1] as $value) { $ordered_list->add($value); }
$ordered_list->shift(); // 1
$ordered_list->value(); // [2, 3]
```
indexOf()
```
$ordered_list = new \Vary\OrderedList();
foreach([2, 3, 1] as $value) { $ordered_list->add($value); }
$ordered_list->indexOf(3); // 2
$ordered_list->indexOf(4); // -1
```

Binary Search: Search in Ordered List
--
```
\Vary\Algorithm::binarySearch([-1, 1, 2], 2); // ['index': 2, 'exist': true]
\Vary\Algorithm::binarySearch([-1, 1, 2], 3); // ['index': 2, 'exist': false]
\Vary\Algorithm::binarySearch([-1, 1, 2], -1); // ['index': 0, 'exist': true]
\Vary\Algorithm::binarySearch([-1, 1, 2], -2); // ['index': 0, 'exist': false]
\Vary\Algorithm::binarySearch([-1, 1, 2], 1); // ['index': 1, 'exist': true]
```

Shell Sort: Sort to Ordered List
--
Sort list of numbers
```
$array = new \Vary\ArrayList();
\Vary\Algorithm::shellSort($array->value());
$array->value() // [-30, -1, 0, 1, 10, 100]
```
Customize sorting rules
```
$array = new \Vary\ArrayList(['a', 2, 0, -1, 'b', 'aa', 1]);
\Vary\Algorithm::shellSort($array->value(), function($a, $b) {
  if (is_long($a) && is_long($b)) { return $a < $b; }
  if (is_string($a) && is_string($b)) {
    return strlen($a) === strlen($b) ? $a < $b : strlen($a) < strlen($b);
  }
  return is_string($b);
});
$array->value() // [-1, 0, 1, 2, 'a', 'b', 'aa']
```
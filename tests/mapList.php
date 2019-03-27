<?php
$map = new \Vary\MapList();
if ($map->size() !== 0) { throw new Exception('MapList size error'); }
if ($map->value() !== []) { throw new Exception('MapList value error'); }
$map = new \Vary\MapList(['a' => '111', 'b' => '222', 'cc' => ['1', '22', '333']]);
if ($map->size() !== 3) { throw new Exception('MapList size error'); }
if ($map->value() !== ['a' => '111', 'b' => '222', 'cc' => ['1', '22', '333']]) {
  throw new Exception('MapList value error');
}
$map->setValue(['aaa' => '222', 'bb' => '111', 'c' => ['1', '22', '333'], 'qqq', 'abc']);
if ($map->value() !== ['aaa' => '222', 'bb' => '111', 'c' => ['1', '22', '333'], 'qqq', 'abc']) {
  throw new Exception('MapList value error');
}
if ($map->get('aaa') !== '222') { throw new Exception('MapList get error'); }
if ($map->get(['123']) !== null) { throw new Exception('MapList get error'); }
if ($map->get('ccc') !== null) { throw new Exception('MapList get error'); }
if ($map->get('c') !== ['1', '22', '333']) { throw new Exception('MapList get error'); }
if ($map->get(0) !== 'qqq') { throw new Exception('MapList get error'); }
if ($map->get(2) !== null) { throw new Exception('MapList get error'); }
if ($map->get(null) !== null) { throw new Exception('MapList get error'); }
<?php

$result1 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 3);
if ($result1['exist'] !== true) { throw new Exception('binarySearch1 exist error'); }
if ($result1['index'] !== 3) { throw new Exception('binarySearch1 index error'); }

$result2 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -1);
if ($result2['exist'] !== true) { throw new Exception('binarySearch2 exist error'); }
if ($result2['index'] !== 0) { throw new Exception('binarySearch2 index error'); }

$result3 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -2);
if ($result3['exist'] !== false) { throw new Exception('binarySearch3 exist error'); }
if ($result3['index'] !== 0) { throw new Exception('binarySearch3 index error'); }

$result4 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], -5);
if ($result4['exist'] !== false) { throw new Exception('binarySearch2 exist error'); }
if ($result4['index'] !== 0) { throw new Exception('binarySearch2 index error'); }

$result5 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 12);
if ($result5['exist'] !== true) { throw new Exception('binarySearch5 exist error'); }
if ($result5['index'] !== 9) { throw new Exception('binarySearch5 index error'); }

$result6 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 13);
if ($result6['exist'] !== false) { throw new Exception('binarySearch6 exist error'); }
if ($result6['index'] !== 10) { throw new Exception('binarySearch6 index error'); }

$result7 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 7);
if ($result7['exist'] !== true) { throw new Exception('binarySearch7 exist error'); }
if ($result7['index'] !== 6) { throw new Exception('binarySearch7 index error'); }

$result8 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 8);
if ($result8['exist'] !== false) { throw new Exception('binarySearch8 exist error'); }
if ($result8['index'] !== 7) { throw new Exception('binarySearch8 index error'); }

$result9 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 9);
if ($result9['exist'] !== false) { throw new Exception('binarySearch9 exist error'); }
if ($result9['index'] !== 7) { throw new Exception('binarySearch9 index error'); }

$result10 = \Vary\Algorithm::binarySearch([-1, 1, 2, 3, 4, 6, 7, 10, 11, 12], 10);
if ($result10['exist'] !== true) { throw new Exception('binarySearch10 exist error'); }
if ($result10['index'] !== 7) { throw new Exception('binarySearch10 index error'); }
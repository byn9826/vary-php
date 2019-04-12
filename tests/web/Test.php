<?php

class Test extends \Vary\Model {
  public static function config()
  {
    self::useTable('test_connection');
  }
}
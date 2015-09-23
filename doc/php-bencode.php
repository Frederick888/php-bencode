<?php

/**
 * Created by PhpStorm.
 * User: frederick
 * Date: 9/22/15
 * Time: 1:09 AM
 */

/**
 * Class BItem
 *
 * The base class of all other types.
 * Actually, only parse(), load() and save() are useful at PHP level
 */
class BItem
{
    /**
     * @return string "BItem" is always returned
     */
    function getType()
    {
    }

    /**
     * @param string $ben The string to parse
     * @return BDict|BList|BStr|BInt
     */
    static function parse($ben)
    {
    }

    /**
     * @param string $file The path of the file to be decoded
     * @return BDict|BList|BStr|BInt
     */
    static function load($file)
    {
    }

    /**
     * @param string $file The path of the file to save the encoded content into
     */
    function save($file)
    {
    }
}

/**
 * Class BDict
 *
 * Dictionary of Bencode.
 * The key of a BDict is always BStr while value could be BDict, BList, BStr or BInt.
 * This is handled by std::map in C++
 */
class BDict extends BItem
{
    /**
     * @return string "BDict" is always returned
     */
    function getType()
    {
    }

    /**
     * @param string $key The path of the object to set
     * @param BDict|BList|BStr|BInt $value The object
     *
     * the path is a series of slash split keys
     * for example,
     *      set('key1/0', new BInt(1))
     * will do
     *      (BDict)[ "key1"(BStr) => (BList)[ 0 => THE_NEW_VALUE ] ]
     * and if you want to use a slash in key, use
     *      \/
     * instead, this could be sometimes confusing, pay attention to it
     *
     * what's more, if any parent of the object in the given path does not exist,
     * it will be automatically created
     * the type of the created parent determines on the format of the key
     * for example,
     *      (BDict)[ ]
     *          key1/0', new BInt(1))
     *          ⟱
     *      (BDict)[ "key1"(BStr) => (BList)[ 0 => 0(BInt) ] ]
     *
     *      (BDict)[ ]
     *          ⟱
     *      set('key1/index0', new BInt(1))
     *          ⟱
     *      (BDict)[ "key1"(BStr) => (BDict)[ "index0"(BStr) => 0(BInt) ] ]
     * so be careful if you want a BDict whose keys are integer-only strings
     */
    function set($key, $value)
    {
    }

    /**
     * @param string $key The path of the object to get
     * @return BDict|BList|BStr|BInt|null $value The object
     *
     * notice that this function returns a copy rather than a reference
     * so something like
     *      BDict  BList                 BInt
     *      $dict->get('key1')->set('0', new BInt(1))
     * may not work as expected
     * you should use
     *      $dict->set('key1/0', new BInt(1))
     * instead
     */
    function get($key)
    {
    }

    /**
     * @param string $key The path of the object to check
     * @return bool
     * @see BDict::get()
     */
    function has($key)
    {
    }

    /**
     * @param string $key The path of the object to delete
     * @return bool Deleted, true; not found, false
     * @see BDict::get()
     */
    function del($key)
    {
    }

    /**
     * @return array[string] Return all keys
     */
    function getKeys()
    {
    }

    /**
     * @return int Return the length of encoded content
     */
    function length()
    {
    }

    /**
     * @return int Return the number of elements
     */
    function size()
    {
    }

    /**
     * @return array Return the array without meta
     *
     * convert the Bencode to a simple PHP array
     * notice that it may cause data loss,
     * so DO NOT use this function if you're not pretty sure about the content
     *
     * @see BDict::toMetaArray()
     */
    function toArray()
    {
    }

    /**
     * @return array Return the meta array
     *
     * in a meta array, each single element will be described as a array
     * for example,
     * BInt(10) will be
     *      [ '_type' => 'BInt', '_length' => 4, '_data' => 10 ]
     * BDict will be
     *      [ '_type' => 'BDict', '_length' => ..., '_size' => ..., '_data' => [ 'key1' => ..., 'key2' => ..., ... ] ]
     * this is very useful when the Bencode contains empty BDict or BList,
     * with out the _type the only way to distinguish them is to call getType()
     */
    function toMetaArray()
    {
    }

    /**
     * @param string $needle The string or integer to search
     * @param string $mode k[ey], s[tring] or i[nt]
     * @return array The result array containing a series of paths
     */
    function search($needle, $mode)
    {
    }

    /**
     * @return string The encoded content
     *
     * this magic method will be called implicitly when being converted to string
     */
    function __toString()
    {
    }

    /**
     * PHP constructor
     */
    function __construct()
    {
    }

    /**
     * PHP destructor
     */
    function __destruct()
    {
    }
}

/**
 * Class BList
 *
 * List of Bencode
 * This is handled by std::vector in C++
 */
class BList extends BItem
{
    /**
     * @return string "BList" is always returned
     */
    function getType()
    {
    }

    /**
     * @param string $key The path of the object to set
     * @param BDict|BList|BStr|BInt $value The object
     * @see BDict::set()
     *
     * something different is that if the key of a BList is larger than the size of it,
     * the value will be simply pushed to the end of the BList
     *
     * use integer-only-keyed BDict if you need dispersed integers as keys,
     * although they will be implemented as strings
     */
    function set($key, $value)
    {
    }

    /**
     * @param string $key The path of the object to get
     * @return BDict|BList|BStr|BInt|null $value The object
     * @see BDict::get()
     */
    function get($key)
    {
    }

    /**
     * @param string $key The path of the object to check
     * @return bool
     * @see BDict::has()
     */
    function has($key)
    {
    }

    /**
     * @param string $key The path of the object to delete
     * @return bool Deleted, true; not found, false
     * @see BDict::del()
     */
    function del($key)
    {
    }

    /**
     * @return int Return the length of encoded content
     */
    function length()
    {
    }

    /**
     * @return int Return the number of elements
     */
    function size()
    {
    }

    /**
     * @return array Return the array without meta
     * @see BDict::toArray()
     */
    function toArray()
    {
    }

    /**
     * @return array Return the meta array
     * @see BDict::toMetaArray()
     */
    function toMetaArray()
    {
    }

    /**
     * @param string $needle The string or integer to search
     * @param string $mode k[ey], s[tring] or i[nt]
     * @return array The result array containing a series of paths
     */
    function search($needle, $mode)
    {
    }

    /**
     * @return string The encoded content
     *
     * this magic method will be called implicitly when being converted to string
     */
    function __toString()
    {
    }

    /**
     * PHP constructor
     */
    function __construct()
    {
    }

    /**
     * PHP destructor
     */
    function __destruct()
    {
    }
}

/**
 * Class BStr
 *
 * String of Bencode.
 */
class BStr extends BItem
{
    /**
     * @return string "BStr" is always returned
     */
    function getType()
    {
    }

    /**
     * @return string
     */
    function get()
    {
    }

    /**
     * @param string $value
     */
    function set($value)
    {
    }

    /**
     * @return int
     */
    function length()
    {
    }

    /**
     * @return string This will simply return the contained value
     *
     * this is somehow useless to PHP developers
     * it will be called mostly when recursively converting a BDict or BList to array
     */
    function toArray()
    {
    }

    /**
     * @return array
     * @see BDict::toMetaArray()
     */
    function toMetaArray()
    {
    }

    /**
     * @return string
     */
    function __toString()
    {
    }

    /**
     * @param string|null $value
     */
    function __construct($value)
    {
    }
}

/**
 * Class BInt
 *
 * Integer of Bencode.
 */
class BInt extends BItem
{
    /**
     * @return string "BInt" is always returned
     */
    function getType()
    {
    }

    /**
     * @return int
     */
    function get()
    {
    }

    /**
     * @param int $value
     */
    function set($value)
    {
    }

    /**
     * @return int
     */
    function length()
    {
    }

    /**
     * @return int This will simply return the contained value
     *
     * this is somehow useless to PHP developers
     * it will be called mostly when recursively converting a BDict or BList to array
     */
    function toArray()
    {
    }

    /**
     * @return array
     * @see BDict::toMetaArray()
     */
    function toMetaArray()
    {
    }

    /**
     * @return string
     */
    function __toString()
    {
    }

    /**
     * @param int|null $value
     */
    function __construct($value)
    {
    }
}
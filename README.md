php-postal
--------

[![Build Status](https://travis-ci.org/openvenues/php-postal.svg?branch=master)](https://travis-ci.org/openvenues/php-postal)

These are the official PHP bindings to [libpostal](https://github.com/openvenues/libpostal), a C library for fast international street address parsing and normalization.

Usage
-----

```php
$expansions = Postal\Expand::expand_address("Quatre vignt douze Ave des Champs-Élysées");
foreach ($expansions as $expansion) {
    echo "$expansion\n";
}

$parsed = Postal\Parser::parse_address("The Book Club 100-106 Leonard St, Shoreditch, London, Greater London, EC2A 4RH, United Kingdom");
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['component']}\n";
}
```

Installation
------------

Before using the Python bindings, you must install the libpostal C library. Make sure you have the following prerequisites:

**On Linux (Ubuntu)**
```
sudo apt-get install libsnappy-dev autoconf automake libtool python-dev pkg-config
```

**On Mac OSX**
```
sudo brew install snappy autoconf automake libtool pkg-config
```

**Installing libpostal**

```
git clone https://github.com/openvenues/libpostal
cd libpostal
./bootstrap.sh
./configure --datadir=[...some dir with a few GB of space...]
make
sudo make install

# On Linux it's probably a good idea to run
sudo ldconfig
```

To install the PHP extension, run:

```
git clone https://github.com/openvenues/php-postal
cd php-postal
phpize
./configure
make
make install
```

This will build a 'postal.so' shared extension. Add it to php.ini using:

```
[postal]
extension = postal.so
```

Compatibility
-------------

php-postal currently supports PHP 5.5 and 5.6. PHP 7 support is in the works but not implemented yet. HipHop/HHVM uses a different process for extensions (C++) so would be out of scope for this repo.

Tests
-----

After compiling, run:

```
make test
```
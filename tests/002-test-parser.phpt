--TEST--
Check parser
--SKIPIF--
<?php if (!extension_loaded("postal")) print "skip"; ?>
--FILE--
<?php
$parsed = Postal\Parser::parse_address("781 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA");
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['value']}\n";
}
?>
--EXPECT--
house_number: 781
road: franklin ave
suburb: crown heights
city_district: brooklyn
city: nyc
state: ny
postcode: 11216
country: usa
--TEST--
Check parser
--SKIPIF--
<?php if (!extension_loaded("postal")) print "skip"; ?>
--FILE--
<?php
$parsed = Postal\Parser::parse_address("Barboncino Pizza 781 Franklin Ave Crown Heights Brooklyn NYC NY 11216 USA");
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['component']}\n";
}
$parsed = Postal\Parser::parse_address("whole foods ny");
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['component']}\n";
}
$parsed = Postal\Parser::parse_address("Государственный Эрмитаж Дворцовая наб., 34 191186, St. Petersburg, Russia", array("country" => "ru", "language" => "ru"));
foreach ($parsed as $component) {
    echo "{$component['label']}: {$component['component']}\n";
}
?>
--EXPECT--
house: barboncino pizza
house_number: 781
road: franklin ave
suburb: crown heights
city_district: brooklyn
city: nyc
state: ny
postcode: 11216
country: usa
house: whole foods
state: ny
house: государственный эрмитаж
road: дворцовая наб.
house_number: 34
postcode: 191186
city: st. petersburg
country: russia
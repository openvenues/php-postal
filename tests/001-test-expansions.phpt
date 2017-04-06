--TEST--
Check expand
--SKIPIF--
<?php if (!extension_loaded("postal")) print "skip"; ?>
--FILE--
<?php
$expansions = Postal\Expand::expand_address("Quatre vingt douze Ave des Champs-Élysées");
foreach ($expansions as $expansion) {
    echo "$expansion\n";
}
$options = array("languages" => array("fr"), "address_components" => ADDRESS_ANY | ADDRESS_NAME | ADDRESS_STREET);
$expansions = Postal\Expand::expand_address("Quatre vingt douze Ave des Champs-Élysées", $options);
foreach ($expansions as $expansion) {
    echo "$expansion\n";
}
$options = array("languages" => array("de"));
$expansions = Postal\Expand::expand_address("Friedrichstraße 128, Berlin, Germany", $options);
foreach ($expansions as $expansion) {
    echo "$expansion\n";
}
?>
--EXPECT--
92 avenue des champs-elysees
92 avenue des champs elysees
92 avenue des champselysees
92 avenue des champs-elysees
92 avenue des champs elysees
92 avenue des champselysees
friedrichstraße 128 berlin germany
friedrich straße 128 berlin germany

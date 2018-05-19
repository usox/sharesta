tests:force
	hhvm vendor/bin/phpunit --stderr -c phpunit.xml

serve:force
	hhvm -m s -p 8080

force:

tests:force
	hhvm vendor/bin/phpunit -c phpunit.xml

serve:force
	hhvm -m s -p 8080

force:

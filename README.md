[![Build Status](https://travis-ci.org/usox/sharesta.svg?branch=master)](https://travis-ci.org/usox/sharesta)

# Sharesta - Strict HAck RESTful Apis

A micro framework to build simple and easy to use rest-like apis - written in
strict hack ([Hack](http://hacklang.org)).

## Usage

Create a class containing your route configuration, also define a sample controller.

```php
final class Routes implements Usox\Sharesta\RoutesInterface {
	public function registerRoutes(Usox\Sharesta\RouterInterface $router): void {
		$router->get('/', function (Usox\Sharesta\RequestInterface $request): \JsonSerializable {
			return new HomeRoute();
		});
	}
}

final class HomeRoute implements \JsonSerializable {
	public function jsonSerialize(): string {
		return 'Welcome home';
	}
}
```

Now bootstrap sharesta, register your routes and let the application controller handle your requests.

```php
	$factory = new Usox\Sharesta\ApiFactory();
	$router = $factory->createRouter();

	$routes = new Routes();
	$routes->registerRoutes($router);

	$request = $factory->createRequest(
		$router,
		new Map($_SERVER),
		new Map($_GET),
	);
	$factory->createApplication(
		$request,
		$router
	)
	->handle(
		'index.hh' // path to the file
	);
```

## Example

See `example/webroot/index.hh` for an example.

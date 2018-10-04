# Sharesta - Strict HAck RESTful Apis

A micro framework to build simple and easy to use rest-like apis - written in
strict hack ([Hack](http://hacklang.org)).

## Usage

First, build some classes containing your logic..

```php

final class HomeRoute implements \JsonSerializable {

    public function jsonSerialize(): string {
        return 'Welcome home';
    }
}

final class UpdateUserRoute implements \JsonSerializable {

    public function __construct(
        private int $user_id,
        private \Usox\Sharesta\RequestInterface $request
    ): void {
    }

    public function jsonSerialize(): bool {
        // do some magic, e.g. access the request body by $this->request
        return true;
    }

}
```

Now create a class containing your route configurations.

```php
final class Routes implements Usox\Sharesta\RoutesInterface {

    public function registerRoutes(Usox\Sharesta\RouterInterface $router): void {
        $router->get(
            '/',
            (Usox\Sharesta\RequestInterface $request): \JsonSerializable ==> {
                return new HomeRoute();
            }
        );

        /**
         * Get variables from the path (e.g. `http://some.tld/users/123`)
         */
        $router->post(
            '/users/:id',
            (Usox\Sharesta\RequestInterface $request): \JsonSerializable ==> {
                return new UpdateUserRoute(
                    $request->getUriValues('id'),
                    $request->getRequestBody()
                );
            }
        );
    }
}
```

Setup sharesta, register your routes and let the application controller handle your requests.

```php
<?hh // strict

require_once 'vendor/autoload.php';

<<__Entrypoint>>
function main(): noreturn {
	/* HH_IGNORE_ERROR[2050] */ $get_vars = dict($_GET);
	/* HH_IGNORE_ERROR[2050] */ $server_vars = dict($_SERVER);

    $factory = new Usox\Sharesta\ApiFactory();
    $router = $factory->createRouter(
        $server_vars,
        $get_vars
    );

    $routes = new Routes();
    $routes->registerRoutes($router);

    $router->route(
        '' // path to the file. Leave it empty if your server configuration defaults to index.hh
    );

    die(0);
}
```

## Example

See `example/webroot/index.hh` for an example.

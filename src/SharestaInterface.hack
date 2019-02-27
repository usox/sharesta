namespace Usox\Sharesta;

use type Facebook\HackRouter\HttpMethod;

interface SharestaInterface {

  public function registerRoutes(
    vec<classname<Contract\SharestaApplicationInterface>> $handler_list
  ): this;

  public function routeAsync(): Awaitable<void>;
}
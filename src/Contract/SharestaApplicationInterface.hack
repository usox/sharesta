namespace Usox\Sharesta\Contract;

use namespace Usox\Sharesta\Route;

interface SharestaApplicationInterface extends \JsonSerializable {

  public static function registerRoute(
    Route\RouteBuilderInterface $route_builder
  ): Route\RouteInterface;
}
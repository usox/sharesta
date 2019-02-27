namespace Usox\Sharesta\Route;

use type Facebook\HackRouter\HttpMethod;
use type Usox\Sharesta\ResponderType;

interface RouteInterface {

  public function getMethod(): HttpMethod;

  public function getPath(): string;

  public function getResponder(): ResponderType;
}
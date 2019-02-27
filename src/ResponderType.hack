namespace Usox\Sharesta;

use type Facebook\Experimental\Http\Message\ServerRequestInterface;

type ResponderType = (
  function (
    ImmMap<string, string>,
    ServerRequestInterface
  ): \JsonSerializable
);

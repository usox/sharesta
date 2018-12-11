<?hh // strict 

namespace Usox\Sharesta\Example;

require_once '../../vendor/hh_autoload.hh';

use Usox\Sharesta\Sharesta;
use type Facebook\HackRouter\HttpMethod;

final class SampleRouteHandler implements \JsonSerializable {

	public function __construct(
		private ImmMap<string, string> $params
	) {
	}

	public function jsonSerialize(): mixed {
		return 'Welcome home';
	}
}

final class ParamRouteHandler implements \JsonSerializable {

	public function __construct(
		private ImmMap<string, string> $params
	) {
	}

	public function jsonSerialize(): mixed {
		return 'Welcome '.$this->params->get('foobar');
	}
}

<<__EntryPoint>>
async function main(): Awaitable<noreturn> {
	$sharesta = new Sharesta();

	$sharesta->registerRoute(
		HttpMethod::GET,
		'/',
		($params) ==> new SampleRouteHandler($params)
	);
	$sharesta->registerRoute(
		HttpMethod::GET,
		'/{foobar}',
		($params) ==> new ParamRouteHandler($params)
	);

	await $sharesta->routeAsync();

	die(0);
}

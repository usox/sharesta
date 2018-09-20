<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;
use function Usox\HackMock\{mock, prospect};
use HH\Lib\Str;

class RouterTest extends \Facebook\HackTest\HackTest {

	private string $base_path = 'some-nice-path';

	private string $default_result = 'roedlbroem';

	public function testRouterMatchesRequestCorrectly(): void {
		$result = mock(\JsonSerializable::class);
		$api_factory = mock(ApiFactoryInterface::class);
		$response = mock(ResponseInterface::class);
		$request = mock(RequestInterface::class);

		$header = 'some-fancy-header';

		prospect($response, 'send')
			->with(vec[$header])
			->times(1);

		prospect($result, 'jsonSerialize')
			->times(1)
			->andReturn($this->default_result);

		prospect($api_factory, 'createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(1)
			->andReturn($response);

		prospect($request, 'setRouteParameters')
			->with(dict['id' => '12', 'field' => 'name'])
			->times(1);
		prospect($request, 'getRoute')
			->times(1)
			->with($this->base_path)
			->andReturn('users/12/name');
		prospect($request, 'getHttpMethod')
			->times(1)
			->andReturn('GET');

		$router = new Router(
			$api_factory,
			$request
		);

		$router->register('users/:id/:field', function($request) use ($result) { return $result; });
		$router->addResponseHeader($header);

		$router->route($this->base_path);
	}

	public function testRouterCatchesDefaultExceptionCorrectly(): void {
		$api_factory = mock(ApiFactoryInterface::class);
		$response = mock(ResponseInterface::class);
		$request = mock(RequestInterface::class);

		prospect($response, 'send')
			->with(vec[])
			->times(1);

		prospect($request, 'getRoute')
			->times(1)
			->with($this->base_path)
			->andThrow(new \Exception());

		prospect($api_factory, 'createResponse')
			->with(Router::HTTP_INTERNAL_SERVER_ERROR, 'Internal server error')
			->times(1)
			->andReturn($response);

		$router = new Router(
			$api_factory,
			$request
		);

		$router->route($this->base_path);
	}

	public function testRouterFailsToMatchRequestCorrectly(): void {
		$result = mock(\JsonSerializable::class);
		$api_factory = mock(ApiFactoryInterface::class);
		$response = mock(ResponseInterface::class);
		$request = mock(RequestInterface::class);

		prospect($request, 'getRoute')
			->times(1)
			->with($this->base_path)
			->andReturn('/api/users/12/name');
		prospect($request, 'getHttpMethod')
			->times(1)
			->andReturn('GET');

		prospect($response, 'send')
			->with(vec[])
			->times(1);

		prospect($api_factory, 'createResponse')
			->with(Router::HTTP_NOT_FOUND, 'The requested resource was not found')
			->times(1)
			->andReturn($response);

		$router = new Router(
			$api_factory,
			$request
		);

		$router->register('foo/:id/:field', function($request) use ($result) { return $result; });

		$router->route($this->base_path);
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectly(): void {
		$result = mock(\JsonSerializable::class);
		$api_factory = mock(ApiFactoryInterface::class);
		$response = mock(ResponseInterface::class);
		$request = mock(RequestInterface::class);

		prospect($result, 'jsonSerialize')
			->times(4)
			->andReturn($this->default_result);

		prospect($request, 'setRouteParameters')
			->with(dict['id' => '12', 'field' => 'name'])
			->times(4);

		prospect($response, 'send')
			->with(vec[])
			->times(4);

		prospect($api_factory, 'createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(4)
			->andReturn($response);

		$test = (
			string $route,
			string $route_base,
			string $method
		) ==> {
			prospect($request, 'getRoute')
				->times(1)
				->with($this->base_path)
				->andReturn($route);
			prospect($request, 'getHttpMethod')
				->times(1)
				->andReturn($method);

			$router = new Router(
				$api_factory,
				$request
			);

			$router->register($route_base.'/:id/:field', function($request) use ($result) { return $result; }, $method);

			$router->route($this->base_path);
		};

		$test('gettest/12/name', 'gettest', 'GET');
		$test('posttest/12/name', 'posttest', 'POST');
		$test('puttest/12/name', 'puttest', 'PUT');
		$test('deltest/12/name', 'deltest', 'DELETE');
	}
}

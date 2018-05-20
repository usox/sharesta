<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;
use HH\Lib\Str;
use Usox\HackMock\MockBuilder;

class RouterTest extends \PHPUnit_Framework_TestCase {

	private string $base_path = 'some-nice-path';

	private string $default_result = 'roedlbroem';

	public function testRouterMatchesRequestCorrectly(): void {
		$result = $this->getHackMockBuilder()->prototype(\JsonSerializable::class);
		$api_factory_builder = $this->getHackMockBuilder()->prototype(ApiFactoryInterface::class);
		$response_builder = $this->getHackMockBuilder()->prototype(ResponseInterface::class);
		$request_builder = $this->getHackMockBuilder()->prototype(RequestInterface::class);

		$response_builder
			->expects('send')
			->times(1);

		$result->expects('jsonSerialize')
			->times(1)
			->andReturn($this->default_result);

		$api_factory_builder
			->expects('createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(1)
			->andReturn($response_builder->build());

		$request_builder
			->expects('setRouteParameters')
			->with(dict['id' => 12, 'field' => 'name'])
			->times(1);
		$request_builder
			->expects('getRoute')
			->times(1)
			->andReturn('users/12/name');
		$request_builder
			->expects('getHttpMethod')
			->times(1)
			->andReturn('GET');

		$router = new Router(
			$api_factory_builder->build(),
			$request_builder->build()
		);

		$router->register('users/:id/:field', $result->build());

		$router->route($this->base_path);
	}

	public function testRouterCatchesDefaultExceptionCorrectly(): void {
		$api_factory_builder = $this->getHackMockBuilder()->prototype(ApiFactoryInterface::class);
		$response_builder = $this->getHackMockBuilder()->prototype(ResponseInterface::class);
		$request_builder = $this->getHackMockBuilder()->prototype(RequestInterface::class);

		$response_builder
			->expects('send')
			->times(1);

		$request_builder
			->expects('getRoute')
			->times(1)
			->andThrow(new \Exception());

		$api_factory_builder
			->expects('createResponse')
			->with(Router::HTTP_INTERNAL_SERVER_ERROR, 'Internal server error')
			->times(1)
			->andReturn($response_builder->build());

		$router = new Router(
			$api_factory_builder->build(),
			$request_builder->build()
		);

		$router->route($this->base_path);
	}

	public function testRouterFailsToMatchRequestCorrectly(): void {
		$result_builder = $this->getHackMockBuilder()->prototype(\JsonSerializable::class);
		$api_factory_builder = $this->getHackMockBuilder()->prototype(ApiFactoryInterface::class);
		$response_builder = $this->getHackMockBuilder()->prototype(ResponseInterface::class);
		$request_builder = $this->getHackMockBuilder()->prototype(RequestInterface::class);

		$request_builder
			->expects('getRoute')
			->times(1)
			->andReturn('/api/users/12/name');

		$request_builder
			->expects('getHttpMethod')
			->times(1)
			->andReturn('GET');

		$response_builder
			->expects('send')
			->times(1);

		$api_factory_builder
			->expects('createResponse')
			->with(Router::HTTP_NOT_FOUND, 'The requested resource was not found')
			->times(1)
			->andReturn($response_builder->build());

		$router = new Router(
			$api_factory_builder->build(),
			$request_builder->build()
		);

		$router->register('foo/:id/:field', $result_builder->build());

		$router->route($this->base_path);
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectly(): void {
		$result_builder = $this->getHackMockBuilder()->prototype(\JsonSerializable::class);
		$api_factory_builder = $this->getHackMockBuilder()->prototype(ApiFactoryInterface::class);
		$response_builder = $this->getHackMockBuilder()->prototype(ResponseInterface::class);
		$request_builder = $this->getHackMockBuilder()->prototype(RequestInterface::class);

		$result = $this->getHackMockBuilder()->prototype(\JsonSerializable::class);
		$result->expects('jsonSerialize')
			->times(4)
			->andReturn($this->default_result);

		$result = $result->build();

		$request_builder
			->expects('setRouteParameters')
			->with(dict['id' => 12, 'field' => 'name'])
			->times(4);

		$response_builder
			->expects('send')
			->times(4);

		$api_factory_builder
			->expects('createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(4)
			->andReturn($response_builder->build());

		$test = function (
			string $route,
			string $route_base,
			string $method
			) use (
				$request_builder,
				$api_factory_builder,
				$result
			) {
			$request_builder
				->expects('getRoute')
				->times(1)
				->andReturn($route);
			$request_builder
				->expects('getHttpMethod')
				->times(1)
				->andReturn($method);

			$router = new Router(
				$api_factory_builder->build(),
				$request_builder->build()
			);

			$router->register($route_base.'/:id/:field', $result, $method);

			$router->route($this->base_path);
		};

		$test('gettest/12/name', 'gettest', 'GET');
		$test('posttest/12/name', 'posttest', 'POST');
		$test('puttest/12/name', 'puttest', 'PUT');
		$test('deltest/12/name', 'deltest', 'DELETE');
	}

	public function getHackMockBuilder(): MockBuilder {
		return new MockBuilder();
	}
}

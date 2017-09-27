<?hh // decl
namespace Usox\Sharesta;

class RouterTest extends \PHPUnit_Framework_TestCase {

	public function setUp() {
		$this->result = \Mockery::mock(\JsonSerializable::class);
		$this->callable = function(RequestInterface $request): \JsonSerializable { return $this->result; };
		$this->base_path = 'some-nice-path';
		$this->request = \Mockery::mock(RequestInterface::class);
		$this->api_factory = \Mockery::mock(ApiFactoryInterface::class);
		$this->response = \Mockery::mock(ResponseInterface::class);

		$this->default_result = 'roedlbroem';

		$this->router = new Router(
			$this->api_factory,
			$this->request
		);
	}

	public function testRouterMatchesRequestCorrectly() {
		$this->router->register('users/:id/:field', $this->callable);

		$this->createRequest('users/12/name', 'GET');

		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->once();

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, sprintf('"%s"', $this->default_result))
			->once()
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->once();

		$this->result
			->shouldReceive('jsonSerialize')
			->once()
			->andReturn($this->default_result);

		$this->assertNull(
			$this->router->route($this->base_path)
		);
	}

	public function testRouterCatchesDefaultExceptionCorrectly() {
		$this->request
			->shouldReceive('getRoute')
			->once()
			->andThrow(new \Exception());

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_INTERNAL_SERVER_ERROR, 'Internal server error')
			->once()
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->once();

		$this->router->route($this->base_path);
	}

	public function testRouterFailsToMatchRequestCorrectly() {
		$this->router->register('foo/:id/:field', $this->callable);

		$this->createRequest('/api/users/12/name', 'GET');

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_NOT_FOUND, 'The requested resource was not found')
			->once()
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->once();

		$this->router->route($this->base_path);
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectly() {
		$this->router->register('gettest/:id/:field', $this->callable, 'GET');
		$this->router->register('posttest/:id/:field', $this->callable, 'POST');
		$this->router->register('puttest/:id/:field', $this->callable, 'PUT');
		$this->router->register('deltest/:id/:field', $this->callable, 'DELETE');

		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->times(4);

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, sprintf('"%s"', $this->default_result))
			->times(4)
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->times(4);

		$this->result
			->shouldReceive('jsonSerialize')
			->times(4)
			->andReturn($this->default_result);

		$this->createRequest('gettest/12/name', 'GET');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('posttest/12/name', 'POST');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('puttest/12/name', 'PUT');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('deltest/12/name', 'DELETE');

		$this->assertNull($this->router->route($this->base_path));
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectlyUsingShortcuts() {
		$this->router->get('gettest/:id/:field', $this->callable);
		$this->router->post('posttest/:id/:field', $this->callable);
		$this->router->put('puttest/:id/:field', $this->callable);
		$this->router->delete('deltest/:id/:field', $this->callable);

		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->times(4);

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, sprintf('"%s"', $this->default_result))
			->times(4)
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->times(4);

		$this->result
			->shouldReceive('jsonSerialize')
			->times(4)
			->andReturn($this->default_result);

		$this->createRequest('gettest/12/name', 'GET');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('posttest/12/name', 'POST');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('puttest/12/name', 'PUT');

		$this->assertNull($this->router->route($this->base_path));

		$this->createRequest('deltest/12/name', 'DELETE');

		$this->assertNull($this->router->route($this->base_path));
	}

	private function createRequest(string $route, string $http_method): void {
		$this->request->shouldReceive('getRoute')->once()->andReturn($route);
		$this->request->shouldReceive('getHttpMethod')->once()->andReturn($http_method);
	}
}

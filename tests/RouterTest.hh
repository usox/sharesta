<?hh // decl
namespace Usox\Sharesta;

class RouterTest extends \PHPUnit_Framework_TestCase {   

	public function setUp() {
		$this->result = \Mockery::mock(\JsonSerializable::class);
		$this->callable = function(RequestInterface $request): \JsonSerializable { return $this->result; };
		$this->base_path = 'some-nice-path';
	}

	public function testRouterMatchesRequestCorrectly() {
		$router = new Router();

		$router->register('users/:id/:field', $this->callable);

		$this->assertSame(
			$this->result,
			$router->route($this->createRequest('users/12/name', 'GET'), $this->base_path)
		);
	}

	public function testRouterFailsToMatchRequestCorrectly() {
		$this->setExpectedException(
			Exception\ServerException::class,
			'The requested resource was not found'
		);
		$router = new Router();

		$router->register('foo/:id/:field', $this->callable);
		$router->route($this->createRequest('/api/users/12/name', 'GET'), $this->base_path);
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectly() {
		$router = new Router();

		$router->register('gettest/:id/:field', $this->callable, 'GET');
		$router->register('posttest/:id/:field', $this->callable, 'POST');
		$router->register('puttest/:id/:field', $this->callable, 'PUT');
		$router->register('deltest/:id/:field', $this->callable, 'DELETE');

		$this->assertSame($this->result, $router->route($this->createRequest('gettest/12/name', 'GET'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('posttest/12/name', 'POST'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('puttest/12/name', 'PUT'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('deltest/12/name', 'DELETE'), $this->base_path));
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectlyUsingShortcuts() {
		$router = new Router();

		$router->get('gettest/:id/:field', $this->callable);
		$router->post('posttest/:id/:field', $this->callable);
		$router->put('puttest/:id/:field', $this->callable);
		$router->delete('deltest/:id/:field', $this->callable);

		$this->assertSame($this->result, $router->route($this->createRequest('gettest/12/name', 'GET'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('posttest/12/name', 'POST'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('puttest/12/name', 'PUT'), $this->base_path));
		$this->assertSame($this->result, $router->route($this->createRequest('deltest/12/name', 'DELETE'), $this->base_path));
	}

	public function testRouterObtainsRouteParamsCorrectly() {
		$router = new Router();

		$router->register('users/:id/:field', $this->callable);
		$router->route($this->createRequest('users/12/name', 'GET'), $this->base_path);
		$routeParams = $router->getRouteParameters();

		$this->assertEquals($routeParams->get('id'), '12');
		$this->assertEquals($routeParams->get('field'), 'name');
	}

	private function createRequest(string $route, string $http_method) {
		$request = \Mockery::mock(RequestInterface::class);
		$request->shouldReceive('getRoute')->once()->andReturn($route);
		$request->shouldReceive('getHttpMethod')->once()->andReturn($http_method);
		return $request;
	}
}

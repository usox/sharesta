<?hh // decl
namespace Usox\Sharesta;

class RouterTest extends \PHPUnit_Framework_TestCase {   

	public function setUp() {
		$this->result = \Mockery::mock(\JsonSerializable::class);
		$this->callable = function(RequestInterface $request): \JsonSerializable { return $this->result; };
		$this->base_path = 'some-nice-path';
		$this->request = \Mockery::mock(RequestInterface::class);

		$this->router = new Router($this->request);
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

		$this->assertSame(
			$this->result,
			$this->router->route($this->base_path)
		);
	}

	public function testRouterFailsToMatchRequestCorrectly() {
		$this->setExpectedException(
			Exception\NotFoundException::class,
			'The requested resource was not found'
		);
		$this->router->register('foo/:id/:field', $this->callable);

		$this->createRequest('/api/users/12/name', 'GET');

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

		$this->createRequest('gettest/12/name', 'GET');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('posttest/12/name', 'POST');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('puttest/12/name', 'PUT');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('deltest/12/name', 'DELETE');

		$this->assertSame($this->result, $this->router->route($this->base_path));
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

		$this->createRequest('gettest/12/name', 'GET');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('posttest/12/name', 'POST');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('puttest/12/name', 'PUT');

		$this->assertSame($this->result, $this->router->route($this->base_path));

		$this->createRequest('deltest/12/name', 'DELETE');

		$this->assertSame($this->result, $this->router->route($this->base_path));
	}

	private function createRequest(string $route, string $http_method): void {
		$this->request->shouldReceive('getRoute')->once()->andReturn($route);
		$this->request->shouldReceive('getHttpMethod')->once()->andReturn($http_method);
	}
}

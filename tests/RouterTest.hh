<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;
use HH\Lib\Str;

class RouterTest extends \PHPUnit_Framework_TestCase {

	private ?\JsonSerializable $result;

	private ?RequestInterface $request;

	private ?ApiFactoryInterface $api_factory;

	private ?ResponseInterface $response;

	private ?Router $router;

	private string $base_path = 'some-nice-path';

	private string $default_result = 'roedlbroem';

	public function setUp(): void {
		$this->result = $this->createMock(\JsonSerializable::class);
		// UNSAFE
		$this->request = \Mockery::mock(RequestInterface::class);
		$this->api_factory = \Mockery::mock(ApiFactoryInterface::class);
		$this->response = \Mockery::mock(ResponseInterface::class);

		$this->router = new Router(
			$this->api_factory,
			$this->request
		);
	}

	public function testRouterMatchesRequestCorrectly(): void {
		$this->router?->register('users/:id/:field', $this->getCallable());

		$this->createRequest('users/12/name', 'GET');

		// UNSAFE
		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->once();

		$this->result
			?->expects(static::once())
			?->method('jsonSerialize')
			?->willReturn($this->default_result);

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->once()
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->once();

		expect(
			$this->router?->route($this->base_path)
		)
		->toBeNull();
	}

	public function testRouterCatchesDefaultExceptionCorrectly(): void {
		// UNSAFE
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
		
		expect(
			$this->router?->route($this->base_path)
		)
		->toBeNull();
	}

	public function testRouterFailsToMatchRequestCorrectly(): void {
		$this->router?->register('foo/:id/:field', $this->getCallable());

		$this->createRequest('/api/users/12/name', 'GET');

		// UNSAFE
		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_NOT_FOUND, 'The requested resource was not found')
			->once()
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->once();

		expect(
			$this->router?->route($this->base_path)
		)
		->toBeNull();
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectly(): void {
		$this->router?->register('gettest/:id/:field', $this->getCallable(), 'GET');
		$this->router?->register('posttest/:id/:field', $this->getCallable(), 'POST');
		$this->router?->register('puttest/:id/:field', $this->getCallable(), 'PUT');
		$this->router?->register('deltest/:id/:field', $this->getCallable(), 'DELETE');

		// UNSAFE
		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->times(4);

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(4)
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->times(4);

		$this->result
			?->expects(static::exactly(4))
			?->method('jsonSerialize')
			?->willReturn($this->default_result);

		$this->createRequest('gettest/12/name', 'GET');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('posttest/12/name', 'POST');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('puttest/12/name', 'PUT');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('deltest/12/name', 'DELETE');

		expect($this->router?->route($this->base_path))->toBeNull();
	}

	public function testRouterMatchesSpecificHTTPMethodCorrectlyUsingShortcuts(): void {
		$this->router?->get('gettest/:id/:field', $this->getCallable());
		$this->router?->post('posttest/:id/:field', $this->getCallable());
		$this->router?->put('puttest/:id/:field', $this->getCallable());
		$this->router?->delete('deltest/:id/:field', $this->getCallable());

		// UNSAFE
		$this->request
			->shouldReceive('setRouteParameters')
			->with(\Mockery::on(function ($value) {
				return $value->toArray() == ['id' => 12, 'field' => 'name'];
			}))
			->times(4);

		$this->api_factory
			->shouldReceive('createResponse')
			->with(Router::HTTP_OK, Str\format('"%s"', $this->default_result))
			->times(4)
			->andReturn($this->response);

		$this->response
			->shouldReceive('send')
			->times(4);

		$this->result
			?->expects(static::exactly(4))
			?->method('jsonSerialize')
			?->willReturn($this->default_result);

		$this->createRequest('gettest/12/name', 'GET');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('posttest/12/name', 'POST');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('puttest/12/name', 'PUT');

		expect($this->router?->route($this->base_path))->toBeNull();

		$this->createRequest('deltest/12/name', 'DELETE');

		expect($this->router?->route($this->base_path))->toBeNull();
	}

	private function createRequest(string $route, string $http_method): void {
		// UNSAFE
		$this->request->shouldReceive('getRoute')->once()->andReturn($route);
		$this->request->shouldReceive('getHttpMethod')->once()->andReturn($http_method);
	}

	private function getCallable(): (function(RequestInterface): \JsonSerializable) {
		return function(RequestInterface $request): \JsonSerializable {
			if ($this->result !== null) {
				return $this->result;
			}
			throw new \Exception();
		};
	}
}

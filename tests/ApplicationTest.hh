<?hh
namespace Usox\Sharesta;

use Mockery as m;

class ApplicationTest extends \PHPUnit_Framework_TestCase {

	public function setUp() {
		$this->api_factory = m::mock(ApiFactoryInterface::class);
		$this->request = m::mock(RequestInterface::class);
		$this->router = m::mock(RouterInterface::class);

		$this->application = new Application(
			$this->api_factory,
			$this->request,
			$this->router
		);
	}

	public function testHandleSendsNotFoundResponse() {
		$this->setExpectedResponse(404, 'The requested resource was not found');

		$this->router
			->shouldReceive('matchRequest')
			->with($this->request)
			->once()
			->andReturn(null);

		$this->assertNull(
			$this->application->handle()
		);
	}

	public function testHandleBadRequestResponse() {
		$response = 'roedlbroem';

		$exception = new Exception\RequestException($response);

		$this->setExpectedResponse(400, $response);

		$this->router
			->shouldReceive('matchRequest')
			->with($this->request)
			->once()
			->andThrow($exception);

		$this->assertNull(
			$this->application->handle()
		);
	}

	public function testHandleSendInternalServerErrorResponse() {
		$this->setExpectedResponse(500, 'The provided route callback is not callable');

		$not_callable = 'really-not-callable';

		$this->router
			->shouldReceive('matchRequest')
			->with($this->request)
			->once()
			->andReturn($not_callable);

		$route_parameter = Map{};

		$this->router
			->shouldReceive('getRouteParameters')
			->once()
			->andReturn($route_parameter);

		$this->request
			->shouldReceive('setRouteParameters')
			->with($route_parameter)
			->once();

		$this->assertNull(
			$this->application->handle()
		);
	}

	public function testHandleSendOkResponse() {
		$this->setExpectedResponse(200, 'OK');

		$callable = function (RequestInterface $request) {
			return 'OK';
		};

		$this->router
			->shouldReceive('matchRequest')
			->with($this->request)
			->once()
			->andReturn($callable);

		$route_parameter = Map{};

		$this->router
			->shouldReceive('getRouteParameters')
			->once()
			->andReturn($route_parameter);

		$this->request
			->shouldReceive('setRouteParameters')
			->with($route_parameter)
			->once();

		$this->assertNull(
			$this->application->handle()
		);
	}

	private function setExpectedResponse($code, $message) {
		$response = m::mock(ResponseInterface::class);
		$response
			->shouldReceive('send')
			->once();

		$this->api_factory
			->shouldReceive('createResponse')
			->with($code, $message)
			->once()
			->andReturn($response);
	}
}

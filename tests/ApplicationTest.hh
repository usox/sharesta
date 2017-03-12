<?hh // partial
namespace Usox\Sharesta;

use Mockery as m;

class ApplicationTest extends \PHPUnit_Framework_TestCase {

	public function setUp() {
		$this->api_factory = m::mock(ApiFactoryInterface::class);
		$this->request = m::mock(RequestInterface::class);
		$this->router = m::mock(RouterInterface::class);

		$this->base_path = 'a-nice-path';

		$this->application = new Application(
			$this->api_factory,
			$this->request,
			$this->router
		);
	}

	/**
	 * @dataProvider handleDataProvider
	 */
	public function testHandleCreatesCorrectResponse(
		int $status_code,
		string $exception_class,
		string $error_message
	): void {
		$exception = new $exception_class($error_message);

		$this->router
			->shouldReceive('route')
			->with($this->request, $this->base_path)
			->once()
			->andThrow($exception);

		$this->setExpectedResponse($status_code, $error_message);

		$this->assertNull(
			$this->application->handle($this->base_path)
		);
	}

	public function handleDataProvider() {
		$error_message = 'a-fancy-error-message';

		return [
			[404, Exception\NotFoundException::class, $error_message],
			[400, Exception\RequestException::class, $error_message],
			[500, Exception\ServerException::class, $error_message],
			[500, \Exception::class, 'Internal server error'],
		];
	}

	public function testHandleSendOkResponse() {
		$message = 'a-fancy-message';

		$this->setExpectedResponse(200, $message);

		$this->router
			->shouldReceive('route')
			->with($this->request, $this->base_path)
			->once()
			->andReturn($message);

		$this->assertNull(
			$this->application->handle($this->base_path)
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

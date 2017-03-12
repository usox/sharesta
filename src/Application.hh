<?hh // strict
namespace Usox\Sharesta;

final class Application implements ApplicationInterface {

	public function __construct(
		private ApiFactoryInterface $api_factory,
		private RequestInterface $request,
		private RouterInterface $router
	): void {
	}

	public function handle(): void {
		try {
			$this->handleRequest();
		} catch (Exception\NotFoundException $e) {
			$this->sendResponse(404, $e->getMessage());
		} catch (Exception\RequestException $e) {
			$this->sendResponse(400, $e->getMessage());
		} catch (Exception\ServerException $e) {
			$this->sendResponse(500, $e->getMessage());
		}
	}

	private function sendResponse(int $status_code, mixed $body): void {
		$this->api_factory
			->createResponse($status_code, $body)
			->send();	
	}

	private function handleRequest(): void {
		$callable = $this->router->matchRequest($this->request);
		if ($callable === null) {
			throw new Exception\NotFoundException('The requested resource was not found');
		}
		$this->request->setRouteParameters($this->router->getRouteParameters());

		if (!is_callable($callable)) {
			throw new Exception\ServerException('The provided route callback is not callable');
		}
		$this->sendResponse(200, $callable($this->request));
	}
}

<?hh // strict
namespace Usox\Sharesta;

final class Application implements ApplicationInterface {

	public function __construct(
		private RequestInterface $request,
		private RouterInterface $router
	): void {
	}

	public function execute(): void {
		$closure = $this->router->matchRequest($this->request);
		if ($closure === null) {
			throw new Exception\NotFoundException('The requested resource was not found');
		}
		$this->request->setRouteParameters($this->router->getRouteParameters());

		if (is_callable($closure)) {
			$response = new Response();
			$response->send(200, $closure($this->request));
		} else {
			throw new Exception\ServerException('This route callback was not callable.');
		}
	}
}

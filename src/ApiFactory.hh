<?hh // strict
namespace Usox\Sharesta;

final class ApiFactory implements ApiFactoryInterface {

	public function createRouter(
		ImmMap<string,string> $get_vars,
		ImmMap<string,string> $server_vars
	): RouterInterface {
		return new Router(
			new Request(
				$get_vars,
				$server_vars,
				new RequestBody()
			)
		);
	}

	public function createApplication(
		RouterInterface $router
	): ApplicationInterface {
		return new Application(
			$this,
			$router
		);
	}

	public function createResponse(int $status_code, mixed $body): ResponseInterface {
		return new Response($status_code, $body);
	}
}

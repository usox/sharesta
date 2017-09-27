<?hh // strict
namespace Usox\Sharesta;

final class ApiFactory implements ApiFactoryInterface {

	public function createRouter(
		ImmMap<string,string> $get_vars,
		ImmMap<string,string> $server_vars
	): RouterInterface {
		return new Router(
			$this,
			new Request(
				$get_vars,
				$server_vars,
				new RequestBody()
			)
		);
	}

	public function createResponse(int $status_code, ?string $body): ResponseInterface {
		return new Response($status_code, $body);
	}
}

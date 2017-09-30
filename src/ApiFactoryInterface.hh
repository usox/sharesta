<?hh // strict
namespace Usox\Sharesta;

interface ApiFactoryInterface {

	public function createRouter(
		ImmMap<string,string> $get_vars,
		ImmMap<string,string> $server_vars
	): RouterInterface;

	public function createResponse(int $status_code, ?string $body): ResponseInterface;
}

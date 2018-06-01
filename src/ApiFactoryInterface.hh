<?hh // strict
namespace Usox\Sharesta;

interface ApiFactoryInterface {

	public function createRouter(
		dict<string,string> $get_vars,
		dict<string,string> $server_vars
	): RouterInterface;

	public function createResponse(int $status_code, ?string $body): ResponseInterface;
}
